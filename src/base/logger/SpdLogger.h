#pragma once

#include <memory>

#include <config/IConfig.h>
#include <logger/ILogger.h>

class SpdLogger : public ILogger
{
public:
    static std::shared_ptr<SpdLogger> Instance();
    static void InitLogger(const IConfigPtr &config);
    virtual void LogInfo(const char *msg) override;
    virtual void LogWarning(const char *msg) override;
    virtual void LogError(const char *msg) override;

private:
    SpdLogger();
};
