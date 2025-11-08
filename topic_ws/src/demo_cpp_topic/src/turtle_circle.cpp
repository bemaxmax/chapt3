#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>

using namespace std::chrono_literals;


class TurtleCircle: public rclcpp::Node
{
private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher;
    rclcpp::TimerBase::SharedPtr timer;

public:
    explicit TurtleCircle(const std::string& node_name):Node(node_name)
    {
        publisher = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);
        timer = this->create_wall_timer(1000ms,std::bind(&TurtleCircle::timer_callback,this));
    }

    void timer_callback()
    {
        auto msg = geometry_msgs::msg::Twist();
        msg.linear.x = 1.0;
        msg.angular.z =0.5;
        publisher->publish(msg);
    }
};

int main(int argc, char* argv[]){
    rclcpp::init(argc,argv);
    auto turtle_pub = std::make_shared<TurtleCircle>("pub_node");
    //TurtleCircle turtle_pub = TurtleCircle("pub_node");不能直接创建栈对象！！
    rclcpp::spin(turtle_pub);
    rclcpp::shutdown();
    return 0;
}