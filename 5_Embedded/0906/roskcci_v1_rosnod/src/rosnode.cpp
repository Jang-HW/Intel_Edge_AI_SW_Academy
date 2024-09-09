#include "../include/roskcci/rosnode.h"

RosNode::RosNode(int argc, char** argv) : init_argc(argc), init_argv(argv)
{
    QSettings topic_setting("rosnode", "settings");
    goal_topic =
        topic_setting.value("topic/topic_init_pose", "move_base_simple/goal").toString();
}

RosNode::~RosNode(){
    /*
    if (ros::isStarted()) {
        ros::shutdown();
        ros::waitForShutdown();
    }
    wait();
    */
    ros::shutdown();
}

bool RosNode::init()
{
    ros::init(init_argc, init_argv, "rosnode",
              ros::init_options::AnonymousName);
    if (!ros::master::check()) {
        return false;
    }
    ROS_INFO("%s", "create node");

    ros::start();
    startHandle();
    start();
    return true;
}

void RosNode::startHandle()
{
  ros::NodeHandle nh;
  set_pub =
        nh.advertise<geometry_msgs::PoseWithCovarianceStamped>("initialpose", 1000);
  goal_pub =
        nh.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1000);

}

void RosNode::set_goal(QString frame, double x, double y, double z, double w) {
  geometry_msgs::PoseStamped goal;
  ros::Rate loop_rate(10);

  goal.header.frame_id = frame.toStdString();

  goal.header.stamp = ros::Time::now();
  goal.pose.position.x = x;
  goal.pose.position.y = y;
  goal.pose.position.z = 0;
  goal.pose.orientation.z = z;
  goal.pose.orientation.w = w;

  set_pub.publish(goal);

  ros::spinOnce();

  loop_rate.sleep();
}

void RosNode::go_goal(QString frame, double x, double y, double z, double w) {
  geometry_msgs::PoseStamped goal;
  ros::Rate loop_rate(10);

  goal.header.frame_id = frame.toStdString();

  goal.header.stamp = ros::Time::now();
  goal.pose.position.x = x;
  goal.pose.position.y = y;
  goal.pose.position.z = 0;
  goal.pose.orientation.z = z;
  goal.pose.orientation.w = w;

  goal_pub.publish(goal);

  ros::spinOnce();

  loop_rate.sleep();
}

