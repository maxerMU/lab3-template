#include "CancelRentPrep.h"

#include "context/ApiGatewayContext.h"
#include <logger/LoggerFactory.h>

void CancelRentPrep::Init(const IRequestHandlerContextPtr &context)
{
    ApiGatewayContextPtr apiGatewayContext = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    apiGatewayContext->SetRequestType(ApiGatewayContext::CancelRent);
    apiGatewayContext->GetProcessInfo().cancelRentRequest.rentUid = m_rentUid;
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

IClientServerRoute::ResponceType CancelRentPrep::ProcessResponse(const IResponsePtr &)
{
    LoggerFactory::GetLogger()->LogError("CancelRentPrep::ProcessResponse unexpected call");

    return IClientServerRoute::END_ROUTE;
}
