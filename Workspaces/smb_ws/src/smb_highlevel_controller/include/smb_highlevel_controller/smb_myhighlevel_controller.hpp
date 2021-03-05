#pragma once

#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <std_srvs/SetBool.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PointStamped.h>
#include <visualization_msgs/Marker.h>
#include <tf/transform_listener.h>
#include <cmath>

namespace smb_highlevel_controller {

	class smb_myhighlevel_controller{
	public:
		smb_myhighlevel_controller(ros::NodeHandle& nodeHandle);
		virtual ~smb_myhighlevel_controller();
		void scanCallback(const sensor_msgs::LaserScan& scan);
		//void lidarCallback(const sensor_msgs::PointCloud2& cloudpoints);
		bool start_stop_command_callback(std_srvs::SetBool::Request &requist, std_srvs::SetBool::Response &response);
		void getparameter();

	private:
		ros::NodeHandle nodeHandle_;
		std::string topic_scan_;
		std::string service_name_;
		//std::string topic_rslidar_points_;
		int queue_size_;
		float P_gain_;
		float x_vel_;
		bool stop_flag_;

		ros::Subscriber scansubscriber_;
		ros::Publisher pillar_publisher_;
		ros::Publisher marker_punlisher_;
		geometry_msgs::Twist pillarcommand;
		ros::ServiceServer start_stop_command_;

		visualization_msgs::Marker marker;
		tf::TransformListener tf_listener_;
		geometry_msgs::PointStamped pose_lidarframe;
		geometry_msgs::PointStamped pose_odom;
		ros::Publisher pillar_marker_punlisher_;
		ros::Subscriber lidarsubscriber_; 
	};

} 