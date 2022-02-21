#include "head/codec.h"

#include "muduo/base/Logging.h"
#include "muduo/base/Mutex.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/TcpClient.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

// 这部分是要被连接的
class ChatClient : noncopyable
{
public:
  ChatClient(EventLoop *loop, const InetAddress &serverAddr)
      : client_(loop, serverAddr, "ChatClient"),
        codec_(std::bind(&ChatClient::onStringMessage, this, _1, _2, _3))
  {
    // 连上以后的回调函数
    client_.setConnectionCallback(
        std::bind(&ChatClient::onConnection, this, _1));
    // 收到数据的回调函数
    client_.setMessageCallback(
        std::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
    client_.enableRetry();
  }
  // 连接上了
  void connect()
  {
    client_.connect();
  }
  // 断开连接
  void disconnect()
  {
    client_.disconnect();
  }
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

  void onStringMessage(const TcpConnectionPtr &,
                       const string &message,
                       Timestamp)
  {
    printf("<<< %s\n", message.c_str());
  }

  TcpClient client_;
  LengthHeaderCodec codec_;
  MutexLock mutex_;
  TcpConnectionPtr connection_ GUARDED_BY(mutex_);
};

int main(int argc, char *argv[])
{
  LOG_INFO << "pid = " << getpid();
  // 第一个变量是 ip地址，第二个变量是port号
  if (argc > 2)
  {
    EventLoopThread loopThread;
    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    InetAddress serverAddr(argv[1], port);

    // 生成一个客户端对象
    ChatClient client(loopThread.startLoop(), serverAddr);
    // 去连接一下
    client.connect();
    std::string line;
    // 读一下，要发的数据
    while (std::getline(std::cin, line))
    {
      client.write(line);
    }
    // 断开连接
    client.disconnect();
    CurrentThread::sleepUsec(1000 * 1000); // wait for disconnect, see ace/logging/client.cc
  }
  else
  {
    // 教一下用户怎么用
    printf("Usage: %s host_ip port\n", argv[0]);
  }
}
