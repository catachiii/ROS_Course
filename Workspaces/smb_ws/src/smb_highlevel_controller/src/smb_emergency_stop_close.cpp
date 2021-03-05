#include <ros/ros.h>
#include <std_srvs/SetBool.h>
#include <sensor_msgs/LaserScan.h>


class emg_stop{
public:
  emg_stop(ros::NodeHandle& nodeHandle){
    scansubscriber_ = nodeHandle.subscribe("/scan", 10, &emg_stop::scanCallback, this);
    client_ = nodeHandle.serviceClient<std_srvs::SetBool>("/smb_highlevel_controller/start_stop_command");
  };

  virtual ~emg_stop(){};

  void scanCallback(const sensor_msgs::LaserScan& scan){
    float smallestdistance;
    for (int i = 0; i < scan.ranges.size(); i++){
      if (smallestdistance > scan.ranges[i]){
        smallestdistance = scan.ranges[i];
      };
    };

    std_srvs::SetBool stopit;
    if (smallestdistance < 1.0) {
      stopit.request.data = false;
      if (client_.call(stopit)) {
        ROS_INFO("Response msg: %s", stopit.response.message.c_str());
      } else {
        ROS_ERROR("Failed to call service");
      };
    } else {
      stopit.request.data = true;
    };
  };

private:
  ros::NodeHandle nodeHandle_;
  ros::Subscriber scansubscriber_;
  ros::ServiceClient client_;
};


int main(int argc, char **argv){
  ros::init(argc, argv, "smb_emergency_stop_close");
  ros::NodeHandle nodeHandle("~");
  emg_stop emergency_stop(nodeHandle);
  ros::spin();
  return 0;
};

