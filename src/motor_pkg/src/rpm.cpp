#include <rclcpp/rclcpp.hpp>
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float64.hpp"
#include <vector>
#include <cstdlib>
#include <chrono>

using namespace std::chrono_literals;

class RPMNode : public rclcpp ::Node
{
    public:
        RPMNode():Node("rpm_pub_node")
        {
            this->declare_parameter<double>("rpm_val", RPM_DEFAULT_VALUE);
            publisher_  = this->create_publisher<std_msgs::msg::Float64>(
                    "rpm_value",10
                ); 
            timer_ = this->create_wall_timer(
                100ms,
            std::bind(&RPMNode::publish_rpm, this)
            );
            RCLCPP_INFO(this->get_logger() , "Initialize RPMNode ");
        }
    private:
        void publish_rpm()
        {
            auto message = std_msgs::msg::Float64();
            rclcpp::Parameter param_object= this->get_parameter("rpm_val"); 
            message.data = param_object.as_double();
            publisher_->publish(message);
        }
        double RPM_DEFAULT_VALUE = 100.0; 
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
};


int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RPMNode>());
    rclcpp::shutdown();
    
    return 0;
}