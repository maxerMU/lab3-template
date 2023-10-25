#pragma once

#include <memory>

#include <config/IConfig.h>
#include <logger/ILogger.h>

class LoggerFactory
{
public:
    static void InitLogger(const IConfigPtr &config);
    static ILoggerPtr GetLogger();
    LoggerFactory() = delete;
};
