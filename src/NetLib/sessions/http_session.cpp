#include "http_session.h"

#include <logger/LoggerFactory.h>

#include "beast_req.h"
#include "beast_resp.h"
#include "http_read_awaiter.h"
#include "http_write_awaiter.h"
#include "std_future.hpp"

std::function<void()> ServerHandCreator = [] {};

HttpServerSession::HttpServerSession(const std::shared_ptr<IServerReqHandlerCreator> &creator)
    : m_handlerCreator(creator)
{
}

HttpServerSession::~HttpServerSession()
{
    LoggerFactory::GetLogger()->LogInfo("sever session finished");
}

HttpServerSessionCreator::HttpServerSessionCreator(const std::shared_ptr<IServerReqHandlerCreator> &creator)
    : m_handlerCreator(creator)
{
}

std::future<void> HttpServerSession::Run(tcp::socket sock)
{
    while (true)
    {
        auto handler_ = m_handlerCreator->CreateHandler();
        std::pair<http::request<http::string_body>, size_t> read_res = co_await HttpAsyncReadRequest(sock);
        if (read_res.second == 0)
        {
            // TODO check sockets close session
            break;
        }
        http::request<http::string_body> req = read_res.first;

        std::shared_ptr<IRequest> req_ptr(new BeastReq(req));
        handler_->HandleRequest(req_ptr);

        std::shared_ptr<IResponse> resp(new BeastResp());
        handler_->MakeResponse(resp);

        auto res = MakeBeastResp(resp);
        co_await HttpAsyncWriteResponse(sock, res);
        // std::cout << "write to socket" << bytes_write << std::endl;
    }

    sock.close();
}
