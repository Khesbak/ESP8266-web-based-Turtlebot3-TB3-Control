# ESP8266-web-based-Turtlebot3-TB3-Control
* General info:
        code and procedure for controlling a simulated TB3-Burger type robot under ROS. The control is done from web server (https://cayenne.mydevices.com).
        The ESP2866 connects with two server ports. One with ROS and the other with Cayenne web server port.
        It communicates in bi-directional communication connecting ROS simulated TB3 and Cayenne web server.
* Tasks monitored and parameters controlled:
        Monitoring:
        1- Robot Linear speed monitoring.
        2- Robot Angular speed monitoring.
        Controlling:
        1-Manual (x,y) TB3 position control.
        2-Manual (w,z) prientation control.
        3-Rotation angle (Theata).
        4-plan setting with (x,y) destination.
* Implementation:
        The application carried out on UBUNTU 18.04.
* Technical Info:
         * Subscribing to: nav_msgs/Odometry
         * Publiching:       geometry_msgs/Twist
         * This intend to connect to a Wifi Access Point and a rosserial TCP socket server1.
         * You can launch the rosserial socket server with (rosrun rosserial_python serial_node.py tcp)
         * The default port is 11411 as server 1
         * This intend to connect to URL cayenne.mydevices.com as MQTT server 2
         * This URL will control the TB3
         * Run the TB3 RViz through (roslaunch turtlebot3_fake turtlebot3_fake.launch)
         * You can controll the TB3 manually by running (roslaunch turtlebot3_teleop turtlebot3_teleop_key.launch)
         
