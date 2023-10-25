#pragma once

#include <router/IClientServerRoute.h>

class PostRentPost : public IClientServerRoute
{
public:
    virtual void Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t> clients) override;
    virtual void SetRequestParameters(const std::vector<std::string> &params) override;
    virtual void ProcessRequest(const IRequestPtr &request, size_t &clientIndex) override;
    virtual ResponceType ProcessResponse(const IResponsePtr &responseFromClient) override;
    virtual RouteType GetRouteType() const { return RESPONSE_MAKER; }

private:
    IRequestHandlerContextPtr m_context;
};