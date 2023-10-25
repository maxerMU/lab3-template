#pragma once

#include <memory>

class IServerConnection
{
public:
    virtual void Run() = 0;

    virtual ~IServerConnection() = default;
};

using IServerConnectionPtr = std::shared_ptr<IServerConnection>;