#pragma once

#include "context/ApiGatewayContext.h"
#include <router/IClientServerRoute.h>

class GetPaymentRoute : public IClientServerRoute
{
public:
    virtual void Init(const IRequestHandlerContextPtr &context) override;
    virtual void SetRequestParameters(const std::vector<std::string> &params) override;
    virtual void ProcessRequest(const IRequestPtr &request, std::string &clientName) override;
    virtual RollbackType Rollback(const IRequestPtr&, std::string&) override { return NON_CRITICAL; }
    virtual ResponceType ProcessResponse(const IResponsePtr &responseFromClient) override;

private:
    const std::string GET_PAYMENT_BASE_TARGET = "/api/v1/payments";

    ApiGatewayContextPtr m_context;
    size_t m_iteration = 0;
};