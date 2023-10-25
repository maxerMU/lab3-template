#ifndef READ_AWAITER_H
#define READ_AWAITER_H

#include <coroutine>
#include <string>

#include <network/net_asio.h>

struct HttpReadRequestAwaiter
{
    bool await_ready();

    void await_suspend(std::coroutine_handle<> h);

    std::pair<http::request<http::string_body>, size_t> await_resume();

    tcp::socket &sock_;

    beast::flat_buffer buf_ = beast::flat_buffer();
    http::request<http::string_body> req_ = http::request<http::string_body>();
    error_code ec_ = error_code();
    size_t bytes_read_ = 0;
};

HttpReadRequestAwaiter HttpAsyncReadRequest(tcp::socket &socket);

struct HttpReadResponseAwaiter
{
    bool await_ready();

    void await_suspend(std::coroutine_handle<> h);

    http::response<http::string_body> await_resume();

    tcp::socket &sock_;

    beast::flat_buffer buf_ = beast::flat_buffer();
    http::response<http::string_body> resp_ = http::response<http::string_body>();
    error_code ec_ = error_code();
    size_t bytes_read_ = 0;
};

HttpReadResponseAwaiter HttpAsyncReadResponse(tcp::socket &socket);

#endif // READ_AWAITER_H
