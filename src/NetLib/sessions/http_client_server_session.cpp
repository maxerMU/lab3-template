#include "http_client_server_session.h"

#include <logger/LoggerFactory.h>

#include "beast_req.h"
#include "beast_resp.h"
#include "http_read_awaiter.h"
#include "http_write_awaiter.h"
#include "std_future.hpp"

HttpClientServerSessionCreator::HttpClientServerSessionCreator(
    const std::shared_ptr<IClientServerReqHandlerCreator> &creator)
    : m_creator(creator)
{
}

HttpClientServerSession::HttpClientServerSession(const std::shared_ptr<IClientServerReqHandlerCreator> &creator)
    : m_handlerCreator(creator)
{
}

std::future<void> HttpClientServerSession::Run(tcp::socket server_sock,
                                               const std::vector<std::shared_ptr<tcp::socket>> &clients_sock)
{
    while (true)
    {
        std::pair<http::request<http::string_body>, size_t> read_res = co_await HttpAsyncReadRequest(server_sock);
        if (read_res.second == 0)
        {
            // TODO check sockets close session
            break;
        }
        http::request<http::string_body> req = read_res.first;
        // std::cout << "read from socket" << std::endl;

        auto handler_ = m_handlerCreator->CreateHandler();

        std::shared_ptr<IRequest> req_ptr(new BeastReq(req));
        auto state = handler_->HandleRequest(req_ptr);

        while (state == IClientServerReqHandler::RES_CONTINUE)
        {
            std::shared_ptr<IRequest> server_req(new BeastReq());
            size_t next_client;

            state = handler_->GetNextRequest(server_req, next_client);
            if (state == IClientServerReqHandler::RES_REPEAT)
            {
                state = IClientServerReqHandler::RES_CONTINUE;
                continue;
            }
            else if (state == IClientServerReqHandler::RES_END)
            {
                break;
            }

            auto beast_req = MakeBeastReq(server_req);
            LoggerFactory::GetLogger()->LogInfo((std::string("next client: ") + std::to_string(next_client)).c_str());
            // std::cout << "is connected: "
            // << clients_sock[next_client]->is_open() << std::endl;
            // std::cout << req_ptr->GetBody() << std::endl;
            // std::cout << req_ptr->GetTarget() << std::endl;
            co_await HttpAsyncWriteRequest(*(clients_sock[next_client]), beast_req);
            // std::cout << "write to client socket" << std::endl;
            http::response<http::string_body> client_resp =
                co_await HttpAsyncReadResponse(*(clients_sock[next_client]));
            // std::cout << "read from client socket" << std::endl;

            std::shared_ptr<IResponse> client_resp_ptr(new BeastResp(client_resp));
            state = handler_->HandleResponse(client_resp_ptr);
        }

        std::shared_ptr<IResponse> resp(new BeastResp());
        handler_->MakeResponse(resp);

        auto res = MakeBeastResp(resp);

        co_await HttpAsyncWriteResponse(server_sock, res);
        // std::cout << "write to socket: " << bytes_write << std::endl;
    }

    // TODO connect to client sockets on need
    // server_sock.close();
    // for (auto sock : clients_sock)
    // {
    //     sock->close();
    // }
    co_return;
}
