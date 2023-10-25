#pragma once

#include <router/IClientServerRoute.h>

class DummyClientServerRoute : public IClientServerRoute
{
public:
    virtual void Init(const IRequestHandlerContextPtr& context, const std::map<std::string, size_t> clients) override;
    virtual void SetRequestParameters(const std::vector<std::string>& params) override;
    virtual void ProcessRequest(const IRequestPtr& request, size_t& clientIndex) override;
    virtual ResponceType ProcessResponse(const IResponsePtr& responseFromClient) override;

private:
    IRequestHandlerContextPtr m_context;
    std::map<std::string, size_t> m_clientsIndexes;
};