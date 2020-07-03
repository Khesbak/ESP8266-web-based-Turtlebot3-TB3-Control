# ESP8266-web-based-Turtlebot3-TB3-Control
* General info:
        code and procedure for controllinmg a simulated TB3-Burger type robot under ROS. The control is done from web server (https://cayenne.mydevices.com).
        The ESP2866 connects with two server ports. One with ROS and the other with Cayenne web server port.
        It communicates in bi-directional communication connecting ROS simulated TB3 and Cayenne web server.
* Tasks monitored and parametrs controlled:
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
