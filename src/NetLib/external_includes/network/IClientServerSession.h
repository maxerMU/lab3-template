#pragma once

#include <memory>

#include <network/net_asio.h>

class IClientServerSession
{
public:
    virtual ~IClientServerSession() = default;
    virtual std::future<void> Run(tcp::socket server_sock,
                                  const std::vector<std::shared_ptr<tcp::socket>> &clients_sock) = 0;
};

using IClientServerSessionPtr = std::shared_ptr<IClientServerSession>;

class IClientServerSessionCreator
{
public:
    virtual ~IClientServerSessionCreator() = default;

    virtual std::shared_ptr<IClientServerSession> CreateSession() = 0;
};

using IClientServerSessionCreatorPtr = std::shared_ptr<IClientServerSessionCreator>;