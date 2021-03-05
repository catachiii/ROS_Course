#include <ros/ros.h>
#include <std_srvs/SetBool.h>
#include <sensor_msgs/LaserScan.h>

float smallestdistance;

void scanCallback(const sensor_msgs::LaserScan& scan)
{
  for (int i = 0; i < scan.ranges.size(); i++){
    if (smallestdistance > scan.ranges[i]){
      smallestdistance = scan.ranges[i];
    }
  };
  smallestdistance = scan.ranges[0];
};

int main(int argc, char **argv){
  ros::init(argc, argv, "smb_emergency_stop_close");
  ros::NodeHandle nodeHandle;
  ros::Subscriber scansubscriber = nodeHandle.subscribe("/scan", 10, scanCallback);
  ros::ServiceClient client = nodeHandle.serviceClient<std_srvs::SetBool>("/smb_highlevel_controller/start_stop_command");
  std_srvs::SetBool stopit;
  if (smallestdistance < 2) {
    stopit.request.data = false;
  };
  client.call(stopit);
  ros::spin();
};

//still have some problems

