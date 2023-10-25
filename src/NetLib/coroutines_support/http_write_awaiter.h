#ifndef WRITE_AWAITER_H
#define WRITE_AWAITER_H

#include <coroutine>
#include <string>

#include <network/net_asio.h>

struct HttpWriteResponseAwaiter
{
    bool await_ready();

    void await_suspend(std::coroutine_handle<> h);

    size_t await_resume();

    tcp::socket &sock_;
    http::response<http::string_body> res_ = http::response<http::string_body>();

    error_code ec_ = error_code();
    size_t bytes_read_ = 0;
};

HttpWriteResponseAwaiter HttpAsyncWriteResponse(tcp::socket &socket, http::response<http::string_body> res);

struct HttpWriteRequestAwaiter
{
    bool await_ready();

    void await_suspend(std::coroutine_handle<> h);

    size_t await_resume();

    tcp::socket &sock_;
    http::request<http::string_body> res_ = http::request<http::string_body>();

    error_code ec_ = error_code();
    size_t bytes_read_ = 0;
};

HttpWriteRequestAwaiter HttpAsyncWriteRequest(tcp::socket &socket, http::request<http::string_body> res);

#endif // WRITE_AWAITER_H
