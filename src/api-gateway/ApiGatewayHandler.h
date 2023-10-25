#pragma once

#include <config/IConfig.h>
#include <request_handlers/IClientServerReqHandler.h>
#include <router/IClientServerRoute.h>
#include <router/IRequestHandlerContext.h>

class ApiGatewayHandler : public IClientServerReqHandler
{
public:
    ApiGatewayHandler(const IConfigPtr &config);
    virtual state_t HandleRequest(const std::shared_ptr<IRequest> &req) override;
    virtual state_t GetNextRequest(std::shared_ptr<IRequest> &req, size_t &client_index) override;
    virtual state_t HandleResponse(const std::shared_ptr<IResponse> &resp) override;
    virtual void MakeResponse(const std::shared_ptr<IResponse> &resp) override;

private:
    IConfigPtr m_config;
    IRequestHandlerContextPtr m_context;

    size_t m_currentRoute;
    std::vector<IClientServerRoutePtr> m_routes;
};