#ifndef ECHO_SESSION_H
#define ECHO_SESSION_H

#include <network/IServerSession.h>

class EchoSession : public IServerSession, public std::enable_shared_from_this<EchoSession>
{
public:
    EchoSession() = default;

    virtual std::future<void> Run(tcp::socket sock) override;
};

class EchoSessionCreator : public IServerSessionCreator
{
public:
    EchoSessionCreator() = default;
    virtual ~EchoSessionCreator() = default;

    std::shared_ptr<IServerSession> CreateSession() override
    {
        return std::shared_ptr<IServerSession>(new EchoSession());
    }
};

#endif // ECHO_SESSION_H
