#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "image_pkg/srv/capture_image.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <string>



typedef image_pkg::srv::CaptureImage CaptureImageSrv;

class CaptureImageServiceNode : public rclcpp::Node
{
    public:
        CaptureImageServiceNode() : Node("capture_image_service_node"), img_path_(DEFAULT_PATH)
        {
            service_server_ = this->create_service<CaptureImageSrv>(
                "capture_image",
                std::bind(&CaptureImageServiceNode::rotate_capture_image,this,
                std::placeholders::_1, std::placeholders::_2
                )
            );
            RCLCPP_INFO(this->get_logger() , "Initialize Server! ");
        }
    private:
        void rotate_capture_image(const CaptureImageSrv::Request::SharedPtr request,
        CaptureImageSrv::Response::SharedPtr response)
        {
            float degree_req = request->rotation_degree;
            RCLCPP_INFO(this->get_logger() , "Rotation Camera: %.2f degrees", degree_req);
            
            const float available_angles[5] = {-30,-15,0,15,30};
            float smallest_gap= std::abs(degree_req - available_angles[0]);
            float closest_angle = available_angles[0]; 

            for (int i = 0 ; i < 5 ; i++)
            {
                if (smallest_gap > std::abs(degree_req - available_angles[i]))
                {
                    smallest_gap = std::abs(degree_req - available_angles[i]);
                    closest_angle = available_angles[i];
                }
            }

            RCLCPP_INFO(this->get_logger() , "Closest Number: %.2f degrees", closest_angle);

            std::string img_path_ = std::string(IMAGE_DIR) + "/" + std::to_string((int)closest_angle) + ".png";
            RCLCPP_INFO(this->get_logger() , "%s", img_path_.c_str());
            auto image = cv::imread(img_path_);
            auto image_msg_ptr = cv_bridge::CvImage(
                std_msgs::msg::Header(), "bgr8", image).toImageMsg();
            
                if (image.empty()){
                    RCLCPP_ERROR(this->get_logger(), "Cannot load image");
                    return;
                }
                else
                    response->current_image = *image_msg_ptr;
        }
        rclcpp::Service<CaptureImageSrv>::SharedPtr service_server_;
        std::string DEFAULT_PATH = std::string(IMAGE_DIR) + "/-30.png";
        std::string img_path_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CaptureImageServiceNode>());
    rclcpp::shutdown();
}