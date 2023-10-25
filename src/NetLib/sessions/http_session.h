#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <network/IServerSession.h>
#include <request_handlers/IServerReqHandler.h>

class HttpServerSession : public IServerSession
{
public:
    HttpServerSession(const std::shared_ptr<IServerReqHandlerCreator> &creator);
    ~HttpServerSession();

    virtual std::future<void> Run(tcp::socket sock) override;

private:
    http::request<http::string_body> m_req;
    std::shared_ptr<IServerReqHandlerCreator> m_handlerCreator;
};

class HttpServerSessionCreator : public IServerSessionCreator
{
public:
    HttpServerSessionCreator(const std::shared_ptr<IServerReqHandlerCreator> &creator);
    virtual ~HttpServerSessionCreator() = default;

    std::shared_ptr<IServerSession> CreateSession() override
    {
        return std::shared_ptr<IServerSession>(new HttpServerSession(m_handlerCreator));
    }

private:
    std::shared_ptr<IServerReqHandlerCreator> m_handlerCreator;
};

#endif // HTTP_SESSION_H
