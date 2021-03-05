#include <ros/ros.h>
#include <std_srvs/SetBool.h>
#include <sensor_msgs/Imu.h>


class emg_stop_crash{
public:
  emg_stop_crash(ros::NodeHandle& nodeHandle){
    imusubscriber_ = nodeHandle.subscribe("/imu0", 10, &emg_stop_crash::imucallback, this);
    client_ = nodeHandle.serviceClient<std_srvs::SetBool>("/smb_highlevel_controller/start_stop_command");
  };

  virtual ~emg_stop_crash(){};

  void imucallback(const sensor_msgs::Imu& imu){
    absangularvel = 0;
    if (abs(imu.angular_velocity.z) > absangularvel) {
      absangularvel = abs(imu.angular_velocity.z);
    };

    std_srvs::SetBool stopit;
    if (absangularvel > 0.07) {
      stopit.request.data = false;
      if (client_.call(stopit)) {
        ROS_INFO("Response msg: %s", stopit.response.message.c_str());
      } else {
        ROS_ERROR("Failed to call service");
      };
    };
  };

private:
  ros::NodeHandle nodeHandle_;
  ros::Subscriber imusubscriber_;
  ros::ServiceClient client_;
  float absangularvel;
};


int main(int argc, char **argv){
  ros::init(argc, argv, "smb_emergency_stop_crash");
  ros::NodeHandle nodeHandle("~");
  emg_stop_crash emergency_stop(nodeHandle);
  ros::spin();
  return 0;
};

