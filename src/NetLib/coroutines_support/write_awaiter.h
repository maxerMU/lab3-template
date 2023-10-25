#ifndef WRITE_AWAITER_H
#define WRITE_AWAITER_H

#include <coroutine>

#include <network/net_asio.h>

struct WriteAwaiter
{
    bool await_ready();

    void await_suspend(std::coroutine_handle<> h);

    void await_resume();

    tcp::socket &socket_;
    boost::asio::const_buffer buffer_ = boost::asio::const_buffer();

    std::error_code ec_ = std::error_code();
};

WriteAwaiter AsyncWrite(tcp::socket &socket, boost::asio::const_buffer buffer);

#endif // WRITE_AWAITER_H
