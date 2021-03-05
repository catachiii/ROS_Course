#include <ros/ros.h>
#include "smb_highlevel_controller/smb_myhighlevel_controller.hpp"


int main(int argc, char** argv) {
  ros::init(argc, argv, "smb_highlevel_controller");
  ros::NodeHandle nodeHandle("~");
  smb_highlevel_controller::smb_myhighlevel_controller smb_myhighlevel_controller(nodeHandle);
  ros::spin();
  return 0;
}
