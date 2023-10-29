#pragma once

#include "context/ApiGatewayContext.h"
#include <router/IClientServerRoute.h>

class PostRentRoute : public IClientServerRoute
{
public:
    virtual void Init(const IRequestHandlerContextPtr &context) override;
    virtual void SetRequestParameters(const std::vector<std::string> &params) override;
    virtual void ProcessRequest(const IRequestPtr &request, std::string &clientName) override;
    virtual bool Rollback(const IRequestPtr& request, std::string& clientName) override;
    virtual ResponceType ProcessResponse(const IResponsePtr &responseFromClient) override;
    virtual ResponceType ProcessRollbackResponse(const IResponsePtr& responseFromClient) override;

private:
    const std::string RENT_BASE_TARGET = "/api/v1/rental";

    ApiGatewayContextPtr m_context;

    std::string m_postedRentUid;
};