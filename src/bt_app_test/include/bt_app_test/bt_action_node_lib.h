#pragma once

// BT
#include <behaviortree_ros2/bt_action_node.hpp>
#include <behaviortree_ros2/bt_topic_sub_node.hpp>
// ROS
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/logger.hpp>
// self defined message
#include "btcpp_ros2_interfaces/action/navigation.hpp"
//message
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/twist.hpp>
// tf2 
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <tf2/impl/utils.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2/exceptions.h>

using namespace BT;

namespace BT {
  template <> inline geometry_msgs::msg::TwistStamped convertFromString(StringView str);
  template <> inline int convertFromString(StringView str);
  template <> inline std::deque<int> convertFromString(StringView str);
}

// Test node
class Testing : public BT::StatefulActionNode
{
public:
  Testing(const std::string& name, const BT::NodeConfig& config, std::shared_ptr<rclcpp::Node> node)
    : BT::StatefulActionNode(name, config), node_(node), tf_buffer_(node_->get_clock()), listener_(tf_buffer_)
  {}

  /* Node remapping function */
  static BT::PortsList providedPorts();

  /* Start and running function */
  BT::NodeStatus onStart() override;
  BT::NodeStatus onRunning() override;
  void UpdateRobotPose();

  /* Halt function */
  void onHalted() override;

private:
  int tick_count = 0;
  geometry_msgs::msg::TwistStamped robot_pose_;
  std::shared_ptr<rclcpp::Node> node_;
  tf2_ros::Buffer tf_buffer_;
  tf2_ros::TransformListener listener_;

  std::string input;
};

class LocalizationTemp : public BT::StatefulActionNode
{
public:
  LocalizationTemp(const std::string& name, const BT::NodeConfig& config, std::shared_ptr<rclcpp::Node> node)
    : BT::StatefulActionNode(name, config), node_(node), tf_buffer_(node_->get_clock()), listener_(tf_buffer_)
  {}

  /* Node remapping function */
  static BT::PortsList providedPorts();

  /* Start and running function */
  BT::NodeStatus onStart() override;
  BT::NodeStatus onRunning() override;
  void UpdateRobotPose();

  /* Halt function */
  void onHalted() override;

private:
  int tick_count = 0;
  geometry_msgs::msg::TwistStamped robot_pose_;
  std::shared_ptr<rclcpp::Node> node_;
  tf2_ros::Buffer tf_buffer_;
  tf2_ros::TransformListener listener_;

  std::string input;
};

// Simple navigation node
class NavigationTemp : public BT::RosActionNode<btcpp_ros2_interfaces::action::Navigation>
{
public:
  NavigationTemp(const std::string& name, const BT::NodeConfig& conf,
                   const BT::RosNodeParams& params)
    : RosActionNode<btcpp_ros2_interfaces::action::Navigation>(name, conf, params)
  {}

  /* Node remapping function */
  static PortsList providedPorts();
  bool setGoal(RosActionNode::Goal& goal) override;
  NodeStatus onResultReceived(const WrappedResult& wr) override;
  virtual NodeStatus onFailure(ActionNodeErrorCode error) override;
  NodeStatus onFeedback(const std::shared_ptr<const Feedback> feedback);

private:
};