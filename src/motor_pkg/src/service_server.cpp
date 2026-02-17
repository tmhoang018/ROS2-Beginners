#include "rclcpp/rclcpp.hpp"
#include "motor_pkg/srv/odd_even_check.hpp"


typedef motor_pkg::srv::OddEvenCheck OddEvenCheckSrv;

class OddEvenCheckServiceNode : public rclcpp::Node
{
    public:
        OddEvenCheckServiceNode() : Node("odd_even_check_service_node")
        {
            service_server_ = this->create_service<OddEvenCheckSrv>(
                "odd_even_check",
                std::bind(&OddEvenCheckServiceNode::check_num_odd_even,this,
                std::placeholders::_1, std::placeholders::_2
                )
            );
        }
    private:
        void check_num_odd_even(const OddEvenCheckSrv::Request::SharedPtr request,
        OddEvenCheckSrv::Response::SharedPtr response)
        {
            int remainder = std::abs(request->number) % 2;
            switch (remainder){
                case 0:
                    response->decision = "Even";
                    break;
                case 1:
                    response->decision = "Odd";
            }
        }
        rclcpp::Service<OddEvenCheckSrv>::SharedPtr service_server_;
};


int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OddEvenCheckServiceNode>());
    rclcpp::shutdown();
}