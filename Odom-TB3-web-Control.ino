/*
 * rosserial TurtleBot3 control example reads TB3 position and velocity and sends new velocities control
 * Subscribes to: nav_msgs/Odometry
 * Publich:       geometry_msgs/Twist
 * This intend to connect to a Wifi Access Point and a rosserial TCP socket server1.
 * You can launch the rosserial socket server with (rosrun rosserial_python serial_node.py tcp)
 * The default port is 11411 as server 1
 * This intend to connect to URL cayenne.mydevices.com as MQTT server 2
 * This URL will control the TB3
 * Run the TB3 RViz through (roslaunch turtlebot3_fake turtlebot3_fake.launch)
 * You can controll the TB3 manually by running (roslaunch turtlebot3_teleop turtlebot3_teleop_key.launch)
 * """   This version send the TB3 to a certain point  """"".
 * Modified 8-5-2020
#include <ESP8266WiFi.h>
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <ros.h>
#include <std_msgs/Float64.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
//  Cayenne libraries
#include <CayenneMQTTESP8266.h>
#define CAYENNE_DEBUG
#define CAYENNE_PRNT Serial
//  *************
#include <math.h>     

const char* ssid     = "<Your ssid>";
const char* password = "<Your Password>";
// Set the rosserial socket server IP address
IPAddress server1(192,168,178,51);
// Set the rosserial socket server port
const uint16_t serverPort = 11411;
// Cayenne setting
char* username ="f445c540-de3b-11e5-a056-c5cffe7f75f5";
char* mqtt_passwrd ="be2b75fbb163a71575c4f566db4bb350db465529";
char* client_id ="dad4cdd0-93dd-11ea-a67f-15e30d90bbf4";
//char* client_id ="07410dr0-943c-12ea-b7527-35f1a8f167ba";
// Create an instance of the server
// specify the port to listen on as an argument

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username1[] = "f444c540-dd2b-11e8-a056-c5cffe7f75f9"; //  from Cayenne 
char password1[] = "be2b75fbb133a71575c3f562db4bb350db465529"; //  from Cayenne 
char clientID1[] = "dad4cdd0-95da-11ea-a67f-15e30d90bbf4"; //  from Cayenne 
WiFiServer server2(1883);
ros::NodeHandle nh;
geometry_msgs::Twist TB3control;

float Pos_x;
float Pos_y;
float Orien_z;
float Orien_w;
float Speed_Linear_x;
float Speed_Angular_z;
float Step=1;
float Enable_Rotation;
float Enable;
float E=0;
float Th;
float Theata;
float old;
float Move;
float xx1;
float yy1;
float D=0;
#define PI 3.14159265

void cmd_cb(const nav_msgs::Odometry& odem)
{
  Pos_x = odem.pose.pose.position.x;
  Pos_y = odem.pose.pose.position.y;
  Orien_z = odem.pose.pose.orientation.z;
  Orien_w = odem.pose.pose.orientation.w;
}

ros::Subscriber<nav_msgs::Odometry> sub("odom", cmd_cb);
ros::Publisher pub("cmd_vel", &TB3control);

#define Position_x 1
#define Position_y 2
#define Orientation_z 3
#define Orientation_w 4

void Forward()
{
 TB3control.linear.x  = Speed_Linear_x;
}

void Stop()
{
 TB3control.angular.z  = 0;
 TB3control.linear.x  = 0;
}
void Rotate()
{
 TB3control.angular.z  = Speed_Angular_z;
 TB3control.linear.x  = 0;
}

void setup()
{
  // Use ESP8266 serial to monitor the process
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  // Connect the ESP8266 the the wifi AP
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  

  // Set the connection to rosserial socket server1
  nh.getHardware()->setConnection(server1, serverPort);
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(pub);
  // Another way to get IP
  Serial.print("IP = ");
  Serial.println(nh.getHardware()->getLocalIP());
  // Start the Cayenne server
  server2.begin();
  Serial.println("Server started");
  Cayenne.begin(username,mqtt_passwrd,client_id,ssid,password);
  Cayenne.begin(username1, password1, clientID1,ssid,password);
  
}
CAYENNE_IN(0){
 String value0;
 value0 = (String)getValue.asString();
 Speed_Linear_x=value0.toFloat();
}
CAYENNE_IN(5){
 String value1;
 value1 = (String)getValue.asString();
 Speed_Angular_z=value1.toFloat();
}
CAYENNE_IN(6){
 String value2;
 value2 = (String)getValue.asString();
 Enable=value2.toFloat();
 if (Enable==0) {
  E=0;
  
 }
 if (E==0 & Enable==1) {
  Enable_Rotation=1;
 }
 if (E==0 & Enable==0) {
  Enable_Rotation=0;
 }
 
}
CAYENNE_IN(7){
 
 String value3;
 value3 = (String)getValue.asString();
 Th=value3.toFloat();
 if (Enable==1) {
  Theata=Th;
 } else {
  Theata=0;
 }
 }
CAYENNE_IN(8){
 
 String value4;
 value4 = (String)getValue.asString();
 Move=value4.toFloat();
 
}
CAYENNE_IN(9){
 
 String value5;
 value5 = (String)getValue.asString();
 xx1=value5.toFloat();
 }
CAYENNE_IN(10){
 
 String value6;
 value6 = (String)getValue.asString();
 yy1=value6.toFloat();
 }
void loop()
{
  Cayenne.loop(Position_x);
  Cayenne.loop(Position_y);
  Cayenne.loop(Orientation_z);
  Cayenne.loop(Orientation_w);
  //Serial.println(atan(1));
  //Serial.println(cos(45 * PI/180));
  if (nh.connected()) {
    if (Move==1 & D==0) {
        if (( xx1- Pos_x ) > 0.05) {
          Speed_Linear_x=0.1;
          Forward();
          pub.publish( &TB3control);
        } else {
          Speed_Linear_x=0;
          Stop();
          pub.publish( &TB3control);  
          D=1;
        }
    }
    if (D==1) {
      if ( (Orien_w-cos(90 * PI/360))> 0.01 ) {
        Speed_Angular_z=-0.1;
        Rotate();
        pub.publish( &TB3control);
      } else {
        Stop();
        pub.publish( &TB3control);
        Enable_Rotation==0;
        D=2;
      }
    }

    if (Move==1 & D==2) {
        if (( yy1+ Pos_y ) > 0.05) {
          Speed_Linear_x=0.1;
          
          Forward();
          pub.publish( &TB3control);
        } else {
          Speed_Linear_x=0;
          Stop();
          pub.publish( &TB3control);  
          D=3;
        }
    }

    
    if (Enable_Rotation==0) {
      Forward();
      
      pub.publish( &TB3control);
      old=Orien_w;
  
    } else {
            
      if ( (Orien_w-cos(Theata * PI/360))> 0.01 ) {
      Speed_Angular_z=-0.1;
      Rotate();
      pub.publish( &TB3control);
      } else {
        Stop();
        pub.publish( &TB3control);
        Enable_Rotation==0;
        E=1;
      }
      
    }
  
  } else {
    Serial.println("Not Connected");
  }
  nh.spinOnce();
  delay(1); 
  
}

CAYENNE_OUT(Position_x)
{
  
  // Send x-position value to Cayenne.
  Cayenne.celsiusWrite(Position_x, Pos_x);
  
}

CAYENNE_OUT(Position_y)
{
  
  // Send y-position value to Cayenne.
  Cayenne.celsiusWrite(Position_y, Pos_y);
  
}
CAYENNE_OUT(Orientation_z)
{
  
  // Send orientation values to Cayenne.
  Cayenne.celsiusWrite(Orientation_z, Orien_z);
  
}
CAYENNE_OUT(Orientation_w)
{
  
  // Send the value to Cayenne in Celsius.
  Cayenne.celsiusWrite(Orientation_w, Orien_w);
  
}
