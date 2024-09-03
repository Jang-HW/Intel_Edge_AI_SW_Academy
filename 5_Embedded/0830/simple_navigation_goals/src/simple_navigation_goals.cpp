/*
 Copyright 2021 NCU MATH.
 Developer: Kuo-Shih Tseng (kuoshih@math.ncu.edu.tw)
 Description: This sample code is to send 3 navigation goals in map frame.
This code activate a node "simple_navigation_goals" 
This node publishe goals to /move_base/goal. 
This code also demonstrates how to publish a navigation goal
You can see the navigation in rviz.
 $Revision: 1.0 $,  2021.08.25
 

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  goal.target_pose.header.frame_id = "map"; //"map" for global frame. "base_link" for local frame;
  goal.target_pose.header.stamp = ros::Time::now();

  goal.target_pose.pose.position.x = 0.5;
  goal.target_pose.pose.position.y = 0.5;
  goal.target_pose.pose.orientation.w = 1.0; //Quaternion
  ROS_INFO("Sending goal 1");
  ac.sendGoalAndWait(goal, ros::Duration(10.0,0), ros::Duration(10.0,0));
	
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Goal arrived!");
  else
    ROS_INFO("The base failed to move to goalfor some reason");	
	
  goal.target_pose.pose.position.x = 0.17;
  goal.target_pose.pose.position.y = -0.02;
  goal.target_pose.pose.orientation.w = 1.0;
  ROS_INFO("Sending goal 2");
  ac.sendGoalAndWait(goal, ros::Duration(20.0,0), ros::Duration(20.0,0));
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Goal arrived!");
  else
    ROS_INFO("The base failed to move to goalfor some reason");

/*
  goal.target_pose.pose.position.x = 0.0;
  goal.target_pose.pose.position.y = -0.5;
  goal.target_pose.pose.orientation.w = 1.0;
  ROS_INFO("Sending goal 3");
  ac.sendGoalAndWait(goal, ros::Duration(20.0,0), ros::Duration(20.0,0));

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Goal arrived!");
  else
    ROS_INFO("The base failed to move to goalfor some reason");
*/
  return 0;
}
