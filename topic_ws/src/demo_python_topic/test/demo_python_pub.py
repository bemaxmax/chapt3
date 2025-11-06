import rclpy
from rclpy.node import Node
import requests


class PublisherNode(Node):
    def __init__(self,node_name):
        super().__init__(node_name)
        self.get_logger().info(f"{node_name}启动！")

        

def main():
    rclpy.init()
    pub_node = PublisherNode("")
    rclpy.spin(pub_node)
    rclpy.shutdown()

