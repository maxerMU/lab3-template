#pragma once

#include "context/ApiGatewayContext.h"
#include <router/IClientServerRoute.h>

class GetCarRoute : public IClientServerRoute
{
public:
    virtual void Init(const IRequestHandlerContextPtr &context) override;
    virtual void SetRequestParameters(const std::vector<std::string> &params) override;
    virtual void ProcessRequest(const IRequestPtr &request, std::string &clientName) override;
    virtual ResponceType ProcessResponse(const IResponsePtr &responseFromClient) override;

private:
    const std::string GET_CAR_BASE_TARGET = "/api/v1/cars";

    ApiGatewayContextPtr m_context;

    size_t m_iteration = 0;
};