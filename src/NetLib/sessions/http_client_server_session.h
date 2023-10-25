#ifndef HTTPCLIENTSERVERSESSION_H
#define HTTPCLIENTSERVERSESSION_H

#include <network/IClientServerSession.h>
#include <request_handlers/IClientServerReqHandler.h>

class HttpClientServerSession : public IClientServerSession
{
public:
    HttpClientServerSession(const std::shared_ptr<IClientServerReqHandlerCreator> &creator);

    virtual std::future<void> Run(tcp::socket server_sock,
                                  const std::vector<std::shared_ptr<tcp::socket>> &clients_sock) override;

private:
    std::shared_ptr<IClientServerReqHandlerCreator> m_handlerCreator;
};

class HttpClientServerSessionCreator : public IClientServerSessionCreator
{
public:
    HttpClientServerSessionCreator(const std::shared_ptr<IClientServerReqHandlerCreator> &creator);
    virtual ~HttpClientServerSessionCreator() = default;

    std::shared_ptr<IClientServerSession> CreateSession() override
    {
        return std::shared_ptr<IClientServerSession>(new HttpClientServerSession(m_creator));
    }

private:
    std::shared_ptr<IClientServerReqHandlerCreator> m_creator;
};

#endif // HTTPCLIENTSERVERSESSION_H
