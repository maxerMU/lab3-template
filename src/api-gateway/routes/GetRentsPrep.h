#pragma once

#include <router/IClientServerRoute.h>

class GetRentsPrep : public IClientServerRoute
{
public:
    virtual void Init(const IRequestHandlerContextPtr &context) override;
    virtual void SetRequestParameters(const std::vector<std::string> &params) override;
    virtual void ProcessRequest(const IRequestPtr &request, std::string &clientName) override;
    virtual ResponceType ProcessResponse(const IResponsePtr &responseFromClient) override;
    virtual RouteType GetRouteType() const
    {
        return REQUEST_PREPARE;
    }

private:
    IRequestHandlerContextPtr m_context;

    std::string m_rentUid;
};