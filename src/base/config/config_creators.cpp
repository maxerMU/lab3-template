#include <config/config_creators.h>

#include "config/yamlcpp_config.h"

IConfigPtr CreateYAMLConfig(const std::string &filename)
{
    return std::make_shared<YamlCppConfig>(filename);
}