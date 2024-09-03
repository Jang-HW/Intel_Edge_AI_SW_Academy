#include "ros/ros.h"
#include "cmd_vel_topic/Cmd_vel_msg.h"

#define PUB_NODE_NAME "cmd_vel_topic_pub" // name of node
#define SUB_NODE_NAME "cmd_vel_topic_sub" // name of node:
#define TOPIC_NAME "cmd_vel_topic" // name of topic

void messageCb(const geometry_msgs::Twist& cmd_vel_topic){
    ROS_INFO("linear.x : %f\n", cmd_vel_topic.linear.x);
    ROS_INFO("linear.y : %f\n", cmd_vel_topic.linear.y);
    ROS_INFO("linear.z : %f\n", cmd_vel_topic.linear.z);
    ROS_INFO("angular.x : %f\n", cmd_vel_topic.angular.x);
    ROS_INFO("angular.y : %f\n", cmd_vel_topic.angular.y);
    ROS_INFO("angular.z : %f\n", cmd_vel_topic.angular.z);
}

int main(int argc, char **argv){
    ros::init(argc, argv, SUB_NODE_NAME);
    ros::NodeHandle nh;

    ros::Subscriber cmd_vel_sub =nh.subscribe(TOPIC_NAME, 10, messageCb);
    ros::spin();

    return 0;
}

