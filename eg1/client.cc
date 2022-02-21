#include "head/codec.h"

#include "muduo/base/Logging.h"
#include "muduo/base/Mutex.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/TcpClient.h"

#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace muduo;
using namespace muduo::net;
uint8_t arr[38005];

// 这部分是要被连接的ssss

// 这个从库函数继承了个类noncopyable
class ChatClient : noncopyable
{
public:
  // 构造汉书
  ChatClient(EventLoop *loop, const InetAddress &serverAddr)
      : client_(loop, serverAddr, "ChatClient"),
        codec_(std::bind(&ChatClient::onStringMessage, this, _1, _2, _3))
  // 绑定message函数的几个 传参
  // codec_个变量名字
  {
    // 连上以后的回调函数
    client_.setConnectionCallback(
        // onConnection的传参也被绑定了
        std::bind(&ChatClient::onConnection, this, _1));
    // 收到数据的回调函数
    client_.setMessageCallback(
        // 绑定的外面的on::message函数
        std::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
    client_.enableRetry();
  }
  // 连接上了
  void connect() { client_.connect(); }
  // 断开连接
  void disconnect() { client_.disconnect(); }
  // 发送数据的回调函数
  void write(const StringPiece &message)
  {
    MutexLockGuard lock(mutex_);
    // 看是否连接上
    if (connection_)
    {
      // 连接上就针对连接 发一个消息
      codec_.send(get_pointer(connection_), message);
    }
  }

private:
  void onConnection(const TcpConnectionPtr &conn)
  {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

    MutexLockGuard lock(mutex_);
    if (conn->connected())
    {
      connection_ = conn;
    }
    else
    {
      connection_.reset();
    }
  }

  void onStringMessage(const TcpConnectionPtr &, const string &message,
                       Timestamp)
  {
    printf("<<< %s\n", message.c_str());
  }

  TcpClient client_;
  // LengthHeaderCodec是个类型，所以这部可以调用类型的数据
  LengthHeaderCodec codec_;
  MutexLock mutex_;
  TcpConnectionPtr connection_ GUARDED_BY(mutex_);
};
unsigned char Image_array[480][802] = {};

int main(int argc, char *argv[])
{
  LOG_INFO << "pid = " << getpid();

  if (argc > 2)
  {
    EventLoopThread loopThread;
    // 第一个变量是 ip地址，第二个变量是port号
    // "args": ["192.168.1.10","7"],
    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    InetAddress serverAddr(argv[1], port);

    // 生成一个客户端对象
    // ChatClient看他被复制的传参（serverAddr） argv[1] 是个ip地址

    ChatClient client(loopThread.startLoop(), serverAddr);
    // 去连接一下
    client.connect();
    std::string line;
    while (1)
    {
      if (flag == 1)
      {
        // 这个 image 是个 转成 二维数组
        memcpy(arr, image.c_str(), 384000);
        flag = 0;
        auto a = 1;
        cv::Mat matA(480, 800, CV_8U, (unsigned char *)arr);
        cv::imshow("this", matA);
        cv::waitKey(1);
        flag = 0;
      }
    }
    // 读一下，要发的数据
    while (std::getline(std::cin, line))
    {
      client.write(line);
    }
    // 断开连接
    client.disconnect();
    CurrentThread::sleepUsec(
        1000 * 1000); // wait for disconnect, see ace/logging/client.cc
  }
  else
  {
    // 教一下用户怎么用
    printf("Usage: %s host_ip port\n", argv[0]);
  }
}
