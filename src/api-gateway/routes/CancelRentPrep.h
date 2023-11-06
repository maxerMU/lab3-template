#pragma once

#include <router/IClientServerRoute.h>
#include <config/IConfig.h>
#include "IQueueHandler.h"

class CancelRentPrep : public IClientServerRoute
{
public:
    CancelRentPrep(const IConfigPtr& config, const IQueueHandlerPtr& queueHander);
    virtual void Init(const IRequestHandlerContextPtr &context) override;
    virtual void SetRequestParameters(const std::vector<std::string> &params) override;
    virtual void ProcessRequest(const IRequestPtr &request, std::string& clientName) override;
    virtual RollbackType Rollback(const IRequestPtr& request, std::string& clientName) override;
    virtual ResponceType ProcessResponse(const IResponsePtr &responseFromClient) override;
    virtual RouteType GetRouteType() const { return REQUEST_PREPARE; }

private:
    IRequestHandlerContextPtr m_context;
    IConfigPtr m_config;
    IQueueHandlerPtr m_queueHandler;

    std::string m_rentUid;
    std::string m_username;
};