#include "FinishRentPrep.h"

#include "context/ApiGatewayContext.h"
#include <logger/LoggerFactory.h>

void FinishRentPrep::Init(const IRequestHandlerContextPtr &context)
{
    ApiGatewayContextPtr apiGatewayContext = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    apiGatewayContext->SetRequestType(ApiGatewayContext::FinishRent);
    apiGatewayContext->GetProcessInfo().finishRentRequest.rentUid = m_rentUid;
}

void FinishRentPrep::SetRequestParameters(const std::vector<std::string> &params)
{
    if (params.size() != 1)
        LoggerFactory::GetLogger()->LogError("get rent prep incorrect params");

    m_rentUid = params[0];
}

void FinishRentPrep::ProcessRequest(const IRequestPtr &, std::string &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPrep::ProcessRequest unexpected call");
}

IClientServerRoute::ResponceType FinishRentPrep::ProcessResponse(const IResponsePtr &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPrep::ProcessResponse unexpected call");

    return IClientServerRoute::END_ROUTE;
}
