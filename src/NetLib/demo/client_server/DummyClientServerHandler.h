#pragma once

#include <request_handlers/IClientServerReqHandler.h>
#include <router/IRequestHandlerContext.h>
#include <router/IClientServerRoute.h>
#include <config/IConfig.h>

class DummyClientServerHandler : public IClientServerReqHandler
{
public:
    DummyClientServerHandler(const IConfigPtr &config);
    virtual state_t HandleRequest(const std::shared_ptr<IRequest> &req) override;
    virtual state_t GetNextRequest(std::shared_ptr<IRequest> &req, std::string &clientName) override;
    virtual state_t HandleResponse(const std::shared_ptr<IResponse> &resp) override;
    virtual void MakeResponse(const std::shared_ptr<IResponse> &resp) override;
    virtual state_t ProcessError() override;

private:
    IConfigPtr m_config;
    IRequestHandlerContextPtr m_context;

    size_t m_currentRoute;
    std::vector<IClientServerRoutePtr> m_routes;
};