#ifndef READ_AWAITER_H
#define READ_AWAITER_H

#include <coroutine>
#include <string>

#include <network/net_asio.h>

struct ReadAwaiter
{
    bool await_ready();

    void await_suspend(std::coroutine_handle<> h);

    size_t await_resume();

    tcp::socket &sock_;
    boost::asio::mutable_buffer res_;

    error_code ec_ = error_code();
    size_t bytes_read_ = 0;
};

ReadAwaiter AsyncRead(tcp::socket &socket, boost::asio::mutable_buffer buffer);

#endif // READ_AWAITER_H
