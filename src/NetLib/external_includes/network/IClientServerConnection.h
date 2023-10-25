#pragma once

#include <memory>
#include <config/IConfig.h>
#include <network/net_asio.h>

class IClientServerConnection
{
public:
    virtual void Run() = 0;
    virtual std::shared_ptr<tcp::socket> ConnetClientSocket(const std::string &clientName) = 0;

    virtual ~IClientServerConnection() = default;
};

using IClientServerConnectionPtr = std::shared_ptr<IClientServerConnection>;