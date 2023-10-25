#pragma once

#include <router/IClientServerRoute.h>

#include "context/ApiGatewayContext.h"

class GetRentsRoute : public IClientServerRoute
{
public:
    virtual void Init(const IRequestHandlerContextPtr &context) override;
    virtual void SetRequestParameters(const std::vector<std::string> &params) override;
    virtual void ProcessRequest(const IRequestPtr &request, std::string &clientName) override;
    virtual ResponceType ProcessResponse(const IResponsePtr &responseFromClient) override;

private:
    ApiGatewayContextPtr m_context;

    const std::string GET_RENTS_BASE_TARGET = "/api/v1/rental";
};