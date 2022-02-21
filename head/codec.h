#ifndef MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H
#define MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H

#include "muduo/base/Logging.h"
#include "muduo/net/Buffer.h"
#include "muduo/net/Endian.h"
#include "muduo/net/TcpConnection.h"
int flag;
muduo::string image;
class LengthHeaderCodec : muduo::noncopyable
{
public:
  typedef std::function<void(const muduo::net::TcpConnectionPtr &,
                             const muduo::string &message,
                             muduo::Timestamp)>
      StringMessageCallback;

  explicit LengthHeaderCodec(const StringMessageCallback &cb)
      : messageCallback_(cb)
  {
  }
  // 在这里收数据呢
//zh11111tts

 // hello vim

  void onMessage(const muduo::net::TcpConnectionPtr &conn,
                 muduo::net::Buffer *buf,
                 muduo::Timestamp receiveTime)
  {
    // 起码有四个。说明是收到数据了。
    while (buf->readableBytes() >= kHeaderLen) // kHeaderLen == 4
    {
      // FIXME: use Buffer::peekInt32()
      // 把数据读进来，
      const void *data = buf->peek();
      int32_t be32 = *static_cast<const int32_t *>(data); // SIGBUS
      const int32_t len = muduo::net::sockets::networkToHost32(be32);
      // 检查一下是不是错误数据
//      len > 65536 ||
      if (len < 0)
      {
        LOG_ERROR << "Invalid length " << len;
        conn->shutdown(); // FIXME: disable reading
        break;
      }
      // 可读取的数据，比长度要多了
      else if (buf->readableBytes() >= len + kHeaderLen)
      {
        buf->retrieve(kHeaderLen);
        muduo::string message(buf->peek(), len);
        // 这里把string的message留下来 传到 全局变量
        image = message;
        // strcpy(a, b);
        flag = 1;
        // messageCallback_(conn, message, receiveTime);
        buf->retrieve(len);
      }
      else
      {
        break;
      }
    }
  }

  // FIXME: TcpConnectionPtr
  void send(muduo::net::TcpConnection *conn,
            const muduo::StringPiece &message)
  {
    muduo::net::Buffer buf;
    buf.append(message.data(), message.size());
    int32_t len = static_cast<int32_t>(message.size());
    int32_t be32 = muduo::net::sockets::hostToNetwork32(len);
    buf.prepend(&be32, sizeof be32);
    conn->send(&buf);
  }

private:
  StringMessageCallback messageCallback_;
  const static size_t kHeaderLen = sizeof(int32_t);
};

#endif // MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H
