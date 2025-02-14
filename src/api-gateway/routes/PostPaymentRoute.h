#pragma once

#include "context/ApiGatewayContext.h"
#include <router/IClientServerRoute.h>

class PostPaymentRoute : public IClientServerRoute
{
public:
    virtual void Init(const IRequestHandlerContextPtr &context) override;
    virtual void SetRequestParameters(const std::vector<std::string> &params) override;
    virtual void ProcessRequest(const IRequestPtr &request, std::string &clientName) override;
    virtual RollbackType Rollback(const IRequestPtr& request, std::string& clientName) override;
    virtual ResponceType ProcessResponse(const IResponsePtr &responseFromClient) override;
    virtual ResponceType ProcessRollbackResponse(const IResponsePtr&responseFromClient) override;

private:
    size_t CalculatePrice();
    const std::string PAYMENT_BASE_TARGET = "/api/v1/payments";

    ApiGatewayContextPtr m_context;

    std::string m_postedPaymentUid;
};