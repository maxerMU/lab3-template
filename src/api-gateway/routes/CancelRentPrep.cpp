#include "CancelRentPrep.h"

#include "context/ApiGatewayContext.h"
#include <logger/LoggerFactory.h>
#include <config/base_sections.h>
#include <exceptions/server_exceptions.h>
#include <sstream>

CancelRentPrep::CancelRentPrep(const IConfigPtr& config, const IQueueHandlerPtr &queueHander)
    : m_config(config), m_queueHandler(queueHander)
{
}

void CancelRentPrep::Init(const IRequestHandlerContextPtr &context)
{
    ApiGatewayContextPtr apiGatewayContext = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    apiGatewayContext->SetRequestType(ApiGatewayContext::CancelRent);
    apiGatewayContext->GetProcessInfo().cancelRentRequest.rentUid = m_rentUid;

    m_context = apiGatewayContext;
    headers_t headers = context->GetCurrentRequest()->GetHeaders();
    auto it = headers.find("X-User-Name");
    if (it == headers.end())
    {
        throw UsernameHeaderNotSetException("cancel rent");
    }
    
    m_username = it->second;
}

void CancelRentPrep::SetRequestParameters(const std::vector<std::string> &params)
{
    if (params.size() != 1)
        LoggerFactory::GetLogger()->LogError("get rent prep incorrect params");

    m_rentUid = params[0];
}

void CancelRentPrep::ProcessRequest(const IRequestPtr &, std::string &)
{
    LoggerFactory::GetLogger()->LogError("CancelRentPrep::ProcessRequest unexpected call");
}

IClientServerRoute::RollbackType CancelRentPrep::Rollback(const IRequestPtr &, std::string &)
{
    LoggerFactory::GetLogger()->LogInfo("ROLLBACK Cancel Rent route");

    if (m_rentUid.empty())
    {
        LoggerFactory::GetLogger()->LogWarning("rent uid is empty");
        return IClientServerRoute::SKIP;
    }

    try
    {
        if (!m_queueHandler->Publish(m_rentUid, m_username))
        {
            std::ostringstream oss;
            oss << "failed add to cancel rent queue: " << m_rentUid;
            LoggerFactory::GetLogger()->LogError(oss.str().c_str());
        }

        m_context->GetCurrentResponse()->SetStatus(net::CODE_204);
        m_context->GetCurrentResponse()->SetBody("");
    }
    catch(const std::exception& e)
    {
        LoggerFactory::GetLogger()->LogError((std::string("failed to push to queue cancel rental uid: ") + m_rentUid).c_str());
        throw;
    }

    return IClientServerRoute::NO_REQUEST;
}

IClientServerRoute::ResponceType CancelRentPrep::ProcessResponse(const IResponsePtr &)
{
    LoggerFactory::GetLogger()->LogError("CancelRentPrep::ProcessResponse unexpected call");

    return IClientServerRoute::END_ROUTE;
}
