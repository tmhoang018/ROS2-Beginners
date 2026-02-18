#include "rclcpp/rclcpp.hpp"
#include "image_pkg/srv/capture_image.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <string>


typedef image_pkg::srv::CaptureImage CaptureImageSrv;

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto service_client_node = rclcpp::Node::make_shared("capture_image_client_node");

    auto client = service_client_node-> create_client<CaptureImageSrv>("capture_image");
    auto request = std::make_shared<CaptureImageSrv::Request>();

    std::cout << "Please order your angle for camera rotation: ";
    std::cin >> request->rotation_degree;

    client->wait_for_service();
    auto result = client->async_send_request(request);
    if(rclcpp::spin_until_future_complete(service_client_node, result) ==
        rclcpp::FutureReturnCode::SUCCESS)
    {
        auto cv_ptr = cv_bridge::toCvCopy(result.get()->current_image, "bgr8");
        auto image = cv_ptr->image;
        cv::imshow("Robot Camera Image", image);
        cv::waitKey(0);
    }
    else{
        std::cout << " There was an error processing the request... " << std::endl;
    }

    rclcpp::shutdown();

    return 0;
}