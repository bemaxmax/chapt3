import rclpy
from rclpy.node import Node
import requests
from queue import Queue
from example_interfaces.msg import String

class PublisherNode(Node):
    def __init__(self,node_name):
        super().__init__(node_name)
        self.node_name = node_name
        self.get_logger().info(f"{node_name}启动！")
        self.novel_publisher = self.create_publisher(String,"novel",10)
        self.novel = Queue()
        self.create_timer(5,self.timer_callback)

    def timer_callback(self):
        if self.novel.qsize()>0:
            msg = String()
            msg.data = self.novel.get()
            self.novel_publisher.publish(msg)
            self.get_logger().info(f"{self.node_name}节点发布话题{msg}")

    def download(self, url):
        response = requests.get(url)
        response.encoding = 'utf-8'
        text = response.text
        self.get_logger().info(f"下载{url}，启动！{len(text)}")
        for line in text.splitlines():
            self.novel.put(line)
        
            
def main():
    rclpy.init()
    pub_node = PublisherNode("novel_pub")
    pub_node.download("http://127.0.0.1:8000/novel1.txt")
    rclpy.spin(pub_node)
    rclpy.shutdown()