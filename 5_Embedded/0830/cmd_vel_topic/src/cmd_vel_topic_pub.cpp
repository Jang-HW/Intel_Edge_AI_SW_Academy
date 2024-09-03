#include "ros/ros.h"
#include "cmd_vel_topic/Cmd_vel_msg.h"

#define PUB_NODE_NAME "cmd_vel_topic_pub" // name of node
#define SUB_NODE_NAME "cmd_vel_topic_sub" // name of node
//#define TOPIC_NAME "cmd_vel_topic" // name of topic : cmd_vel_pub
#define TOPIC_NAME "cmd_vel" // name of topic : cmd_vel_pub

float vel_x, vel_y;

int main(int argc, char **argv){
  ros::init(argc, argv, PUB_NODE_NAME);
  ros::NodeHandle nh;
  geometry_msgs::Twist cmd_vel_topic; // variable to publish

  ros::Publisher cmd_vel_pub = nh.advertise<geometry_msgs::Twist>(TOPIC_NAME, 100);
  ros::Rate loop_rate(10);

  cmd_vel_topic.linear.x =0;
//  cmd_vel_topic.linear.y =0;
  cmd_vel_topic.angular.z =0;

  while (ros::ok()){

    std::cout << "input velocity" << std::endl;
    std::cin >> vel_x >> vel_y;

    cmd_vel_topic.linear.x = vel_x;   //max : 0.22
//	cmd_vel_topic.linear.y = vel_y;   //max : 2.84
  cmd_vel_topic.angular.z = vel_y;

    cmd_vel_pub.publish(cmd_vel_topic);
    loop_rate.sleep();// Goes to sleep according to the loop rate defined above.

  }
  return 0;
}

