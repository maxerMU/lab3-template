#ifndef HTTPCLIENTSERVERSESSION_H
#define HTTPCLIENTSERVERSESSION_H

#include <network/IClientServerSession.h>
#include <request_handlers/IClientServerReqHandler.h>

class HttpClientServerSession : public IClientServerSession
{
public:
    HttpClientServerSession(const IClientServerReqHandlerCreatorPtr &creator, const IConfigPtr& config);

    virtual std::future<void> Run(tcp::socket server_sock, const IClientServerConnectionPtr &connection) override;

private:
    IClientServerReqHandlerCreatorPtr m_handlerCreator;
    IConfigPtr m_config;
};

class HttpClientServerSessionCreator : public IClientServerSessionCreator
{
public:
    HttpClientServerSessionCreator(const IClientServerReqHandlerCreatorPtr &creator, const IConfigPtr& config);
    virtual ~HttpClientServerSessionCreator() = default;

    std::shared_ptr<IClientServerSession> CreateSession() override
    {
        return std::shared_ptr<IClientServerSession>(new HttpClientServerSession(m_creator, m_config));
    }

private:
    IClientServerReqHandlerCreatorPtr m_creator;
    IConfigPtr m_config;
};

#endif // HTTPCLIENTSERVERSESSION_H
