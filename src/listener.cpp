#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_listener");

  ros::NodeHandle node;

  ros::Publisher point_vel =
       node.advertise<geometry_msgs::Twist>("cmd_vel", 10);

  tf::TransformListener listener;

  ros::Rate rate(10.0);
  while (node.ok()){
    tf::StampedTransform transform;
    try{
      listener.lookupTransform("/base_link", "/point_num",
                               ros::Time(0), transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
    }

    geometry_msgs::Twist vel_msg;
    
    if(transform.getOrigin().z()<1)
      vel_msg.linear.x = 0;
    else
      vel_msg.linear.x = 0.5 * sqrt(pow(transform.getOrigin().x(), 2) +
                                  pow(transform.getOrigin().y(), 2));
    
    vel_msg.angular.z = 4 * atan2(transform.getOrigin().y(),
                                 transform.getOrigin().x());
    

    point_vel.publish(vel_msg);

    rate.sleep();
  }
  return 0;
};

