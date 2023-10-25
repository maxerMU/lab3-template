#include <logger/LoggerFactory.h>

#include "SpdLogger.h"

void LoggerFactory::InitLogger(const IConfigPtr &config)
{
    SpdLogger::InitLogger(config);
}

ILoggerPtr LoggerFactory::GetLogger()
{
    return SpdLogger::Instance();
}
