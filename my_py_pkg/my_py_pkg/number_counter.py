#!/usr/bin/env python3
import rclpy
from rclpy.node import Node

from example_interfaces.msg import Int64
from example_interfaces.srv import SetBool


class NumberCounterNode(Node): 
    def __init__(self):
        super().__init__("number_counter") 
        self.msg_counter_ = 0
        self.subscriber_ = self.create_subscription(
            Int64, "/number", self.callback_number_counter, 10)
        self.publisher_ = self.create_publisher(Int64, "number_count", 10)
        self.reset_counter_servies_ = self.create_service(SetBool, "reset_counter", self.callback_reset_counter)
        self.get_logger().info("Number Count&Publish has been started.")
        

    def callback_number_counter(self, msg):
        self.msg_counter_ += msg.data
        msg.data = self.msg_counter_
        self.publisher_.publish(msg)
        self.get_logger().info(str(msg.data))

    
    def callback_reset_counter(self, request, response):
        if request.data:
            self.msg_counter_ = 0
            response.success = True
            response.message = "Counter has been reset"
        else:
            response.success = False
            response.message = "counter has not been reset"
        return response


def main(args=None):
    rclpy.init(args=args)
    node = NumberCounterNode()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()