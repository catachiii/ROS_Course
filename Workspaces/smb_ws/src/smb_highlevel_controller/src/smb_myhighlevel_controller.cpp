#include <smb_highlevel_controller/smb_myhighlevel_controller.hpp>

namespace smb_highlevel_controller{

  smb_myhighlevel_controller::smb_myhighlevel_controller(ros::NodeHandle& nodeHandle): nodeHandle_(nodeHandle)
  {
    smb_myhighlevel_controller::getparameter();

    pillar_publisher_=nodeHandle_.advertise<geometry_msgs::Twist>("/cmd_vel",1);
    marker_punlisher_=nodeHandle_.advertise<visualization_msgs::Marker>( "visualization_marker", 0 );
    
    stop_flag_ = false;
    scansubscriber_ = nodeHandle_.subscribe(topic_scan_, queue_size_, &smb_myhighlevel_controller::scanCallback, this);
    
    //pillar_marker_punlisher_=nodeHandle_.advertise<visualization_msgs::Marker>("visualization_marker",0);
    //lidarsubscriber_ = nodeHandle_.subscribe(topic_rslidar_points_, queue_size_, &smb_myhighlevel_controller::lidarCallback, this);

    start_stop_command_ =nodeHandle_.advertiseService(service_name_, &smb_myhighlevel_controller::start_stop_command_callback, this);
  } //constructor

  smb_myhighlevel_controller::~smb_myhighlevel_controller(){} //deconstructor
  
  void smb_myhighlevel_controller::scanCallback(const sensor_msgs::LaserScan& scan)
  {
    //get the point with the smallest distance
    float smallestdistance = scan.ranges[0];
    int smallestdistanceindex = 0;
    for (int i = 0; i < scan.ranges.size(); i++){
      if (smallestdistance > scan.ranges[i]){
        smallestdistance = scan.ranges[i];
        smallestdistanceindex = i;
      }
    };

    //calculate the pillar position in the rslidar frame
    float angle = scan.angle_min + smallestdistanceindex * scan.angle_increment;
    float pillarx = std::cos(angle)*smallestdistance;
    float pillary = std::sin(angle)*smallestdistance;
    //a simple P controller to drive the robot to the pillar

    if (stop_flag_ == false){
      pillarcommand.linear.x = x_vel_; //constant x command from parameters
      pillarcommand.angular.z = angle * P_gain_; //P controller
    } else {
      pillarcommand.linear.x = 0;
      pillarcommand.angular.z = 0;
    };
    pillar_publisher_.publish(pillarcommand); //publish conmmand

    //marker config in the rslidar frame
    marker.header.frame_id = "rslidar";
    marker.header.stamp = ros::Time();
    marker.ns = "marker_ns";
    marker.id = 0;
    marker.type = visualization_msgs::Marker::SPHERE;
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = pillarx;
    marker.pose.position.y = pillary;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
    marker.scale.x = 0.3;
    marker.scale.y = 0.3;
    marker.scale.z = 0.3;
    marker.color.a = 0.5;
    marker.color.r = 0.0;
    marker.color.g = 1.0; //green marker
    marker.color.b = 0.0;
    marker_punlisher_.publish(marker); //publish marker

    /*
    //use a stamped point in rslidar frame to express the pillar point in the rslidar frame
    pose_lidarframe.header.stamp = ros::Time();
    pose_lidarframe.header.frame_id = "rslidar";
    pose_lidarframe.point.x = pillarx;
    pose_lidarframe.point.y = pillary;

    try{
      tf_listener_.waitForTransform("odom", "rslidar", ros::Time(0), ros::Duration(3.0));
      tf_listener_.transformPoint("odom", pose_lidarframe, pose_odom);
    }
    catch(tf::TransformException &ex){
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
    }

    //pillar_marker config in the odom frame
    visualization_msgs::Marker pillar_marker;
    pillar_marker.header.frame_id = "odom";
    pillar_marker.header.stamp = ros::Time();
    pillar_marker.ns = "pillar_marker_ns";
    pillar_marker.id = 0;
    pillar_marker.type = visualization_msgs::Marker::SPHERE;
    pillar_marker.action = visualization_msgs::Marker::ADD;
    pillar_marker.pose.position.x = pose_odom.point.x;
    pillar_marker.pose.position.y = pose_odom.point.y;
    pillar_marker.pose.position.z = pose_odom.point.z;
    pillar_marker.pose.orientation.x = 0;
    pillar_marker.pose.orientation.y = 0;
    pillar_marker.pose.orientation.z = 0;
    pillar_marker.pose.orientation.w = 1;
    pillar_marker.scale.x = 0.3;
    pillar_marker.scale.y = 0.3;
    pillar_marker.scale.z = 0.3;
    pillar_marker.color.a = 0.5;
    pillar_marker.color.r = 1.0; //red marker
    pillar_marker.color.g = 0.0;
    pillar_marker.color.b = 0.0;
    pillar_marker_punlisher_.publish(pillar_marker); //publish pillar_marker
    
    //print messages in the terminator
    ROS_INFO("The position of the pillar in lidar frame is x: %f; y: %f.", pillarx, pillary);
    ROS_INFO("The position of the pillar in the odom frame is x: %f;y: %f;z: %f.\n", pose_odom.point.x, pose_odom.point.y, pose_odom.point.z);
    */
  };

  bool smb_myhighlevel_controller::start_stop_command_callback(std_srvs::SetBool::Request &request, std_srvs::SetBool::Response &response){
    if (request.data == false){
      stop_flag_ = true;
      response.success = true;
      response.message = "Successfully stopped the robot!";
    } else {
      stop_flag_ = false;
      response.success = true;
      response.message = "Successfully started the robot!";
    };
    return true;
  };

  /*
  void smb_myhighlevel_controller::lidarCallback(const sensor_msgs::PointCloud2& cloudpoints)
  {
    int numcloudpoints = cloudpoints.data.size();
    ROS_INFO("The number of points in lidar data: %d", numcloudpoints);
  }
  */

  void smb_myhighlevel_controller::getparameter(){
    if (!nodeHandle_.getParam("topic_scan", topic_scan_)) {
      ROS_ERROR("Could not find topic scan!");
    }
    if (!nodeHandle_.getParam("queue_size", queue_size_)) {
      ROS_ERROR("Could not find queue_size parameter!");
    }
    if (!nodeHandle_.getParam("service_name", service_name_)) {
      ROS_ERROR("Could not find such a service!");
    }
    if (!nodeHandle_.getParam("P_gain", P_gain_)) {
      ROS_ERROR("Could not find P_gain parameter!");
    }
    if (!nodeHandle_.getParam("x_vel", x_vel_)) {
      ROS_ERROR("Could not find x_vel parameter!");
    }
    /*
    if (!nodeHandle_.getParam("topic_rslidar_points", topic_rslidar_points_)) {
      ROS_ERROR("Could not find topic rslidar_points!");
    }
    */
  };

};
