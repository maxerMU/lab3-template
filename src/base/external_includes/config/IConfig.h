#pragma once

#include <memory>
#include <string>
#include <vector>

class IConfig
{
public:
    virtual ~IConfig() = default;
    virtual int GetIntField(const std::vector<std::string> &path) const = 0;
    virtual unsigned int GetUintField(const std::vector<std::string> &path) const = 0;
    virtual std::string GetStringField(const std::vector<std::string> &path) const = 0;

    virtual std::vector<int> GetIntArray(const std::vector<std::string> &path) const = 0;
    virtual std::vector<unsigned int> GetUintArray(const std::vector<std::string> &path) const = 0;
    virtual std::vector<std::string> GetStringArray(const std::vector<std::string> &path) const = 0;
};

using IConfigPtr = std::shared_ptr<IConfig>;