#pragma once

#include <router/IClientServerRoute.h>
#include "context/ApiGatewayContext.h"

class CancelPaymentRoute : public IClientServerRoute
{
public:
    virtual void Init(const IRequestHandlerContextPtr &context) override;
    virtual void SetRequestParameters(const std::vector<std::string> &params) override;
    virtual void ProcessRequest(const IRequestPtr &request, std::string& clientName) override;
    virtual ResponceType ProcessResponse(const IResponsePtr &responseFromClient) override;

private:
    const std::string PAYMENT_BASE_TARGET = "/api/v1/payments";

    ApiGatewayContextPtr m_context;
};