#include <rclcpp/rclcpp.hpp>
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float64.hpp"
#include <iostream>
#include <chrono>
#include <cmath>


using namespace std::chrono_literals;

class SpeedNode : public rclcpp::Node
{
    public:
        SpeedNode(): Node("speed_sub_node")
        {
            this->declare_parameter<double>("radius_val", RADIUS_DEFAULT_VALUE); 
            _rpm_sub = this->create_subscription<std_msgs::msg::Float64>(
                "rpm_value", 10, std::bind(&SpeedNode::get_rpm_callback, this,std::placeholders::_1)
            );

            _speed_pub = this->create_publisher<std_msgs::msg::Float64>(
                "speed_value",10
            );
            _timer = this->create_wall_timer(
            100ms,
        std::bind(&SpeedNode::pub_speed, this)
        );
        RCLCPP_INFO(this->get_logger() , "Initialize SpeedNode ");
        }
    private:
        void pub_speed()
        {
            // const float dimemsion_wheel = 10 ;  // cm
            rclcpp::Parameter radius_wheel = this->get_parameter("radius_val");
            auto speed =   radius_wheel.as_double()*PI*_rpm.data*(0.01F)/ (60.0F); // m/s    PI*d*0.01*RPM/60     
            std_msgs::msg::Float64 msg;
            msg.data = speed;
            _speed_pub->publish(msg);
            RCLCPP_INFO(this->get_logger() , "Speed: %.5f m/s", msg.data);
        }
        void get_rpm_callback(const std_msgs::msg::Float64 & msg)
        {
            _rpm = msg;
            // RCLCPP_INFO(this->get_logger() , "RPM: %.2f rpm", msg.data);
        }
        float PI = M_PI;
        double RADIUS_DEFAULT_VALUE = 0.01; //m 
        std_msgs::msg::Float64 _rpm;
        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr _rpm_sub;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr _speed_pub;
        rclcpp::TimerBase::SharedPtr _timer;

};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SpeedNode>());
    rclcpp::shutdown();
    
    return 0;
}