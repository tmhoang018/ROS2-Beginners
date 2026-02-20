#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "action_pkg/action/navigate.hpp"
#include "geometry_msgs/msg/point.hpp"
#include <iostream>

typedef action_pkg::action::Navigate NavigateAction;
typedef rclcpp_action::ClientGoalHandle<NavigateAction> GoalHandle;
using geometry_msgs::msg::Point;


class NavigateActionClientNode : public rclcpp::Node
{
    public:
        NavigateActionClientNode(): Node("navigate_action_client_node")
        {
            action_client_ = rclcpp_action::create_client<NavigateAction>(
                this, "navigate");
            prompt_user_for_goal();
        }
    
    private:
        void prompt_user_for_goal()
        {
            using std::placeholders::_1; using std::placeholders::_2;
            auto goal_msg = NavigateAction::Goal();

            std::cout <<  "Enter a X-Coordinate to travel to: ";
            std::cin >> goal_msg.goal_point.x;

            std::cout << "Enter a Y-Coordinate to travel to: ";
            std::cin >> goal_msg.goal_point.y;

            std::cout << "Enter a Z-Coordinate to travel to: ";
            std::cin >> goal_msg.goal_point.z;

            this->action_client_->wait_for_action_server();
            std::cout << "Sending Goal" << std::endl;
            auto send_goal_options = rclcpp_action::Client<NavigateAction>::SendGoalOptions();
        
            send_goal_options.goal_response_callback = 
                std::bind(&NavigateActionClientNode::goal_response_callback, this, _1);

            send_goal_options.feedback_callback =           
                std::bind(&NavigateActionClientNode::feedback_callback, this, _1, _2);
        

            send_goal_options.result_callback =           
            std::bind(&NavigateActionClientNode::result_callback, this, _1);
    
        
            this->action_client_->async_send_goal(goal_msg, send_goal_options);
        }

        void goal_response_callback(GoalHandle::SharedPtr future)
        {
            auto goal_handle = future.get();

            if(!goal_handle)
            {
                std::cout << "Goal was rejected by the server" << std::endl;
            }
            else {
                std::cout << " Goal accepted by server, waiting for result..." << std::endl;
            }
        }

        void feedback_callback(
            GoalHandle::SharedPtr future,
            const std::shared_ptr<const NavigateAction::Feedback> feedback)
            {
                (void)future;
                std::cout << "Feedback: "<< feedback-> distance_to_point << std::endl;
            }
        
        void result_callback( const GoalHandle::WrappedResult &result)
        {
            switch ( result.code)
            {
                case rclcpp_action::ResultCode::SUCCEEDED:
                    std::cout << "Time elapsed: " << result.result->elapsed_time << std::endl;
                    break;
                case rclcpp_action::ResultCode::ABORTED:
                std::cout << "Goal was aborted " << std::endl;
                    break;
                case rclcpp_action::ResultCode::CANCELED:
                    std::cout << "Goal was canceled"  << std::endl;
                    break;
                default:
                std::cout << "Unknown result code" << std::endl;
                break;
            }

            rclcpp::shutdown();
        }

        rclcpp_action::Client<NavigateAction>::SharedPtr action_client_;
};


int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<NavigateActionClientNode>());
    rclcpp::shutdown();
}