#pragma once

#include <functional>
#include <memory>

#include <reqresp/reqresp.h>

// TODO разбить по файлам
class IServerReqHandler
{
public:
    virtual ~IServerReqHandler() = default;

    virtual void HandleRequest(const std::shared_ptr<IRequest> &req) = 0;
    virtual void MakeResponse(const std::shared_ptr<IResponse> &resp) = 0;
};

using IServerReqHandlerPtr = std::shared_ptr<IServerReqHandler>;

class IServerReqHandlerCreator
{
public:
    virtual ~IServerReqHandlerCreator() = default;

    virtual std::shared_ptr<IServerReqHandler> CreateHandler() const = 0;
};

using IServerReqHandlerCreatorPtr = std::shared_ptr<IServerReqHandlerCreator>;

template <class T>
concept base_server_handler = std::is_base_of<IServerReqHandler, T>::value;

template <base_server_handler T> class ServerReqHandlerCreator : public IServerReqHandlerCreator
{
public:
    template <typename... Args> ServerReqHandlerCreator(Args... args)
    {
        m_createFunc = [args...]() { return std::make_shared<T>(args...); };
    }

    virtual ~ServerReqHandlerCreator() = default;

    std::shared_ptr<IServerReqHandler> CreateHandler() const override
    {
        return m_createFunc();
    }

private:
    std::function<std::shared_ptr<IServerReqHandler>()> m_createFunc;
};
