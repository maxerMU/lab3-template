#ifndef BASE_SESSION_H
#define BASE_SESSION_H

#include <network/net_asio.h>

#include <memory>

class IServerSession
{
public:
    virtual ~IServerSession() = default;

    virtual std::future<void> Run(tcp::socket sock) = 0;
};

using IServerSessionPtr = std::shared_ptr<IServerSession>;

class IServerSessionCreator
{
public:
    virtual ~IServerSessionCreator() = default;

    virtual IServerSessionPtr CreateSession() = 0;
};

using IServerSessionCreatorPtr = std::shared_ptr<IServerSessionCreator>;

#endif // BASE_SESSION_H
