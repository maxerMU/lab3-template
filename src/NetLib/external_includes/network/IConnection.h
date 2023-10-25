#pragma once

#include <memory>

class IConnection
{
public:
    virtual void Run() = 0;

    virtual ~IConnection() = default;
};

using IConnectionPtr = std::shared_ptr<IConnection>;