#pragma once

#include <boost/asio.hpp>
#include <boost/beast.hpp>

// TODO change namespaces names
namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = boost::beast::http;

using tcp = asio::ip::tcp;
using error_code = boost::system::error_code;
