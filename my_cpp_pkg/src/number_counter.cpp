#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/int64.hpp"
#include "example_interfaces/srv/set_bool.hpp"

class NumberCounterNode : public rclcpp::Node 
{
public:
    NumberCounterNode() : Node("number_counter") 
    {
        subscriber_ = this->create_subscription<example_interfaces::msg::Int64>("number", 10, 
            std::bind(&NumberCounterNode::callbackNumberCounter, this, std::placeholders::_1));
        publisher_ = this->create_publisher<example_interfaces::msg::Int64>("number_count", 10);
        server_ = this->create_service<example_interfaces::srv::SetBool>(
            "reset_counter", 
            std::bind(&NumberCounterNode::callbackSetBool, this, std::placeholders::_1, std::placeholders::_2));
        RCLCPP_INFO(this->get_logger(), "Number Counter has been started.");
                                
    }

private:

    void callbackSetBool(const example_interfaces::srv::SetBool::Request::SharedPtr request,
                            const example_interfaces::srv::SetBool::Response::SharedPtr response)
    {
        if (request->data == true)
        {
            this->counter_ = 0;
            response->success = true;
            response->message = "Counter has been reset";
        }
        else
        {
            response->success = false;
            response->message = "Counter has not been reset";
        }
    }

    void callbackNumberCounter(const example_interfaces::msg::Int64::SharedPtr msg)
    {
        auto publish_msg = example_interfaces::msg::Int64();
        this->counter_ += msg->data;
        publish_msg.data = this->counter_;
        publisher_->publish(publish_msg);
        RCLCPP_INFO(this->get_logger(), "%ld", publish_msg.data);
    }

    rclcpp::Subscription<example_interfaces::msg::Int64>::SharedPtr subscriber_;
    rclcpp::Publisher<example_interfaces::msg::Int64>::SharedPtr publisher_;
    rclcpp::Service<example_interfaces::srv::SetBool>::SharedPtr server_;
    int counter_ = 0;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NumberCounterNode>(); 
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}