
#include "muduo/net/TcpServer.h"
#include "muduo/base/AsyncLogging.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Thread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"

#include <functional>
#include <utility>

#include <stdio.h>
#include <unistd.h>
#include <string>
using namespace muduo;
using namespace muduo::net;
#include <iostream>
using namespace std;

class EchoServer
{
public:
  EchoServer(EventLoop *loop, const InetAddress &listenAddr)
      : loop_(loop),
        server_(loop, listenAddr, "EchoServer")
  {
    server_.setConnectionCallback(
        std::bind(&EchoServer::onConnection, this, _1));
    server_.setMessageCallback(
        std::bind(&EchoServer::onMessage, this, _1, _2, _3));
  }

  void start()
  {
    server_.start();
  }

private:
  void onConnection(const TcpConnectionPtr &conn);

  void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time);

  EventLoop *loop_;
  TcpServer server_;
};
// 服务器就是会被链接的。
void EchoServer::onConnection(const TcpConnectionPtr &conn)
{
  // 被链接的时候就
  // 打印出来对方的
  // IP:端口号

  cout << "remote is " << conn->peerAddress().toIpPort() << endl;
  cout << "local is " << conn->localAddress().toIpPort() << endl;
  if (conn->connected())
  {
    cout << "on!" << endl;
  }
  else
  {
    cout << "off!" << endl;
  }
}
// 收到的回调函数
void EchoServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
{
  string msg(buf->retrieveAllAsString());
  LOG_TRACE << conn->name() << " recv " << msg.size() << " bytes at " << time.toString();
  conn->send(msg);
}

int kRollSize = 500 * 1000 * 1000;

std::unique_ptr<muduo::AsyncLogging> g_asyncLog;

// 异步的输出消息
void asyncOutput(const char *msg, int len)
{
  g_asyncLog->append(msg, len);
}

int main(int argc, char *argv[])
{

  LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
  EventLoop loop;
  cout << atoi(argv[1]) << endl;
  InetAddress listenAddr(atoi(argv[1]));
  // 初始化，一个服务器对象
  EchoServer server(&loop, listenAddr);

  server.start();

  loop.loop();
}

// 看下要不要记录，给个路径
// setLogging(argv[1]);
// void setLogging(const char *argv0)
// {
//   muduo::Logger::setOutput(asyncOutput);
//   char name[256];
//   strncpy(name, argv0, 256);
//   g_asyncLog.reset(new muduo::AsyncLogging(::basename(name), kRollSize));
//   g_asyncLog->start();
// }
