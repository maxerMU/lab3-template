#pragma once

#include <config/IConfig.h>
#include <config/base_sections.h>

#include <functional>
#include <map>
#include <memory>

#include <reqresp/reqresp.h>

// TODO разбить по файлам
class IClientServerReqHandler
{
public:
    enum state_t
    {
        RES_CONTINUE,
        RES_REPEAT,
        RES_END
    };

    virtual ~IClientServerReqHandler() = default;
    virtual state_t HandleRequest(const std::shared_ptr<IRequest> &req) = 0;
    virtual state_t GetNextRequest(std::shared_ptr<IRequest> &req, size_t &client_index) = 0;
    virtual state_t HandleResponse(const std::shared_ptr<IResponse> &resp) = 0;
    virtual void MakeResponse(const std::shared_ptr<IResponse> &resp) = 0;

protected:
    std::map<std::string, size_t> m_clientIndexes;
    void SetClientIndexes(const std::shared_ptr<IConfig> &config)
    {
        m_clientIndexes.clear();
        std::vector<std::string> clients = config->GetStringArray({ClientsSection});

        for (size_t i = 0; i < clients.size(); i++)
            m_clientIndexes[clients[i]] = i;
    }
};

using IClientServerReqHandlerPtr = std::shared_ptr<IClientServerReqHandler>;

class IClientServerReqHandlerCreator
{
public:
    virtual ~IClientServerReqHandlerCreator() = default;

    virtual std::shared_ptr<IClientServerReqHandler> CreateHandler() const = 0;
};

using IClientServerReqHandlerCreatorPtr = std::shared_ptr<IClientServerReqHandlerCreator>;

template <class T>
concept base_client_server_handler = std::is_base_of<IClientServerReqHandler, T>::value;

template <base_client_server_handler T>
class ClientServerReqHandlerCreator : public IClientServerReqHandlerCreator
{
public:
    template <typename... Args> ClientServerReqHandlerCreator(Args... args)
    {
        create_func_ = [args...]() { return std::shared_ptr<IClientServerReqHandler>(new T(args...)); };
    }

    ~ClientServerReqHandlerCreator() = default;

    std::shared_ptr<IClientServerReqHandler> CreateHandler() const override
    {
        return create_func_();
    }

private:
    std::function<std::shared_ptr<IClientServerReqHandler>()> create_func_;
};
