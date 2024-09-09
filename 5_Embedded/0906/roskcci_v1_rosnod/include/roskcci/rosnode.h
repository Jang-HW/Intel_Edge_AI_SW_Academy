#ifndef __ROSNODE__H_
#define __ROSNODE__H_

#include <ros/ros.h>
#include <QThread>

#include <QSettings>
#include <sstream>
#include <string>

#include <std_msgs/String.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

class RosNode : public QThread{
    Q_OBJECT
public:
    RosNode(int argc, char **argv);
    ~RosNode();
    bool init();
    void startHandle();
    void set_goal(QString frame, double x, double y, double z, double w);
    void go_goal(QString frame, double x, double y, double z, double w);
private:
    int init_argc;
    char **init_argv;
    QString goal_topic;
    ros::Publisher set_pub;
    ros::Publisher goal_pub;
    ros::Subscriber cmdVel_sub;
};

#endif
