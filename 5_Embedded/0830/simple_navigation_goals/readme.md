
## Abstract
This sample code is to send 3 navigation goals in map frame.
This code activate a node "simple_navigation_goals" 
This node publishes goals to /move_base/goal. 
This code also demonstrates how to publish a navigation goal
You can see the navigation in rviz.

## About us

Developer:   
* Kuo-Shih Tseng   
Contact: kuoshih@math.ncu.edu.tw   
Date: 2021/08/25   
License: Apache 2.0  

## Install other packages
Make sure you install "turtlebot3_navigation", "turtlebot3_simulations", turtlebot3_slam, "turtlebot3_teleop."  
If you did not install it, copy them from https://github.com/ROBOTIS-GIT/turtlebot3
Then, install the "turtlebot3_simulations"  
$ cd ~/catkin_ws/src/  
$ git clone -b kinetic-devel https://github.com/ROBOTIS-GIT/turtlebot3_simulations.git  
$ cd ~/catkin_ws/  
$ catkin build  

## Install simple_navigation_goals
$ cd catkin_ws/src  
$ git clone https://github.com/kuoshih/simple_navigation_goals  
$ cd ..  
$ catkin_make or catkin build

Or Download this this code to ~\catkin_ws\src.   
Unzip simple_navigation_goals.zip.
  
$ cd catkin_ws  
$ catkin_make or catkin build 

## Run the code   
If you are not familiar with navigation and SLAM of turtlebot3,
please see https://emanual.robotis.com/docs/en/platform/turtlebot3/simulation/#gazebo-simulation
 
$ roslaunch turtlebot3_gazebo turtlebot3_world.launch  # Terminal 1 , open gazebo environment.   
$ roslaunch turtlebot3_navigation turtlebot3_navigation.launch map_file:=$HOME/map.yaml # Terminal 2 ,  Make sure you have map file in HOME.  
$ roslaunch turtlebot3_teleop turtlebot3_teleop_key.launch # Terminal 3 , remote control   
$ rosrun simple_navigation_goals simple_navigation_goals # Terminal 4 , send goals.  

## Visualization
When you activate "turtlebot3_navigation", you will see the rviz.
 

![alt text](https://github.com/kuoshih/simple_navigation_goals/blob/main/document/rviz.png)  

## Edit code  
You can edit src/simple_navigation_goals.cpp for your project.

