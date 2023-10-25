#include "config/config_creators.h"
#include "logger/LoggerFactory.h"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        std::shared_ptr<IConfig> config = CreateYAMLConfig(std::string(argv[1]));
        LoggerFactory::InitLogger(config);
    }

    LoggerFactory::GetLogger()->LogInfo("test info");
    LoggerFactory::GetLogger()->LogWarning("test warning");
    LoggerFactory::GetLogger()->LogError("test error");
    return 0;
}
