#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/msg/pose.hpp"
#include <algorithm>

class TurtleControl: public rclcpp::Node
{
private:
    double target_x{-1.0};
    double target_y{1.0};
    double k{1.0};
    double max_speed{3.0};
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscriber;

public:
    explicit TurtleControl(const std::string& node_name):Node(node_name)
    {
        publisher = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);
        
        subscriber = this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose",10,
        std::bind(&TurtleControl::on_pose_recieved,this,std::placeholders::_1));
        
    }

    void on_pose_recieved(const turtlesim::msg::Pose::SharedPtr Pose)
    {
        auto current_x = Pose->x;
        auto current_y = Pose->y;
        RCLCPP_INFO(get_logger(),"当前：x = %f,当前：y = %f ",current_x,current_y);
        
        auto distance = std::sqrt((target_x - current_x)*(target_x - current_x)
        +(target_y - current_y)*(target_y - current_y));
        
        auto angle = std::atan2((target_y - current_y),(target_x - current_x)) - Pose->theta;
        
        auto msg = geometry_msgs::msg::Twist();
        if (distance>0.1){
            if (fabs(angle)>0.2){
                msg.angular.z = fabs(angle);
            }
            else{
                msg.linear.x = k*distance;
            }
        }
        msg.linear.x = std::min(max_speed,msg.linear.x);
        publisher->publish(msg);
    }
};

int main(int argc, char* argv[]){
    rclcpp::init(argc,argv);
    auto turtle_con = std::make_shared<TurtleControl>("con_node");
    rclcpp::spin(turtle_con);
    rclcpp::shutdown();
    return 0;
}