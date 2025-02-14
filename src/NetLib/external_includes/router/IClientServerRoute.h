#pragma once

#include <functional>
#include <router/IRequestHandlerContext.h>

class IClientServerRoute
{
public:
    virtual ~IClientServerRoute() = default;

    virtual void Init(const IRequestHandlerContextPtr& context) = 0;
    virtual void SetRequestParameters(const std::vector<std::string>& params) = 0;
    virtual void ProcessRequest(const IRequestPtr& request, std::string& clientName) = 0;
    enum RollbackType
    {
        NEED_REQUEST,
        NO_REQUEST,
        NON_CRITICAL,
        SKIP
    };
    virtual RollbackType Rollback(const IRequestPtr&, std::string&) { return SKIP; }

    enum ResponceType
    {
        END_ROUTE,
        REPEAT_ROUTE
    };
    virtual ResponceType ProcessResponse(const IResponsePtr& responseFromClient) = 0;
    virtual ResponceType ProcessRollbackResponse(const IResponsePtr&) { return END_ROUTE; }

    enum RouteType
    {
        DEFAULT_ROUTE,
        REQUEST_PREPARE,
        RESPONSE_MAKER
    };
    virtual RouteType GetRouteType() const { return DEFAULT_ROUTE; }
};

using IClientServerRoutePtr = std::shared_ptr<IClientServerRoute>;

// ==================================

class IClientServerRouteCreator
{
public:
    virtual ~IClientServerRouteCreator() = default;

    virtual IClientServerRoutePtr Create() const = 0;
};

using IClientServerRouteCreatorPtr = std::shared_ptr<IClientServerRouteCreator>;

template <class T>
concept BaseClientServerRoute = std::is_base_of<IClientServerRoute, T>::value;

template <BaseClientServerRoute T>
class ClientServerRouteCreator : public IClientServerRouteCreator
{
public:
    template <typename... Args>
    ClientServerRouteCreator(Args... args)
    {
        m_createFunc = [args...]() { return std::make_shared<T>(args...); };
    }

    virtual ~ClientServerRouteCreator() = default;

    IClientServerRoutePtr Create() const override
    {
        return m_createFunc();
    }

private:
    std::function<IClientServerRoutePtr()> m_createFunc;
};