#pragma once

#include <yaml-cpp/yaml.h>

#include <config/IConfig.h>

class YamlCppConfig : public IConfig
{
public:
    YamlCppConfig(const std::string &filename);

    virtual int GetIntField(const std::vector<std::string> &path) const override;
    virtual unsigned int GetUintField(const std::vector<std::string> &path) const override;
    virtual std::string GetStringField(const std::vector<std::string> &path) const override;

    virtual std::vector<int> GetIntArray(const std::vector<std::string> &path) const override;
    virtual std::vector<unsigned int> GetUintArray(const std::vector<std::string> &path) const override;
    virtual std::vector<std::string> GetStringArray(const std::vector<std::string> &path) const override;

private:
    template <typename T> std::vector<T> GetArray(const std::vector<std::string> &path) const;
    void GetNode(YAML::Node &node, const std::vector<std::string> &path) const;

    YAML::Node m_StartNode;
};

template <typename T> std::vector<T> YamlCppConfig::GetArray(const std::vector<std::string> &path) const
{
    YAML::Node node;
    GetNode(node, path);

    std::vector<T> res;
    for (size_t i = 0; i < node.size(); i++)
        res.push_back(node[i].as<T>());

    return res;
}
