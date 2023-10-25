#pragma once

#include <memory>

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void LogInfo(const char *msg) = 0;
    virtual void LogWarning(const char *msg) = 0;
    virtual void LogError(const char *msg) = 0;
};

using ILoggerPtr = std::shared_ptr<ILogger>;