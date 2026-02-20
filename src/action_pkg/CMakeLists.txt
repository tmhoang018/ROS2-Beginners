cmake_minimum_required(VERSION 3.8)
project(action_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(action_msgs REQUIRED)
find_package(rclcpp_action REQUIRED)
 

#Generate custrom interfac
rosidl_generate_interfaces(${PROJECT_NAME}
  "action/Navigate.action"
  ADD_LINTER_TESTS
  DEPENDENCIES
  sensor_msgs geometry_msgs action_msgs rclcpp_action
  )

ament_export_dependencies(rosidl_default_runtime)


# Set support for using custom interfaces in C++ from this package\
rosidl_get_typesupport_target(cpp_typesupport_target "${PROJECT_NAME}" "rosidl_typesupport_cpp")

add_executable(action_client src/action_client.cpp)
ament_target_dependencies(action_client rclcpp std_msgs sensor_msgs rclcpp_action)
target_link_libraries(action_client "${cpp_typesupport_target}")

add_executable(action_server src/action_server.cpp)
ament_target_dependencies(action_server rclcpp std_msgs sensor_msgs rclcpp_action)
target_link_libraries(action_server "${cpp_typesupport_target}")


#Config for ROS2 cli tools
install(TARGETS
  action_server
  action_client
  DESTINATION lib/${PROJECT_NAME}
)

# install(DIRECTORY srv
#   DESTINATION share/${PROJECT_NAME}
# )


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
