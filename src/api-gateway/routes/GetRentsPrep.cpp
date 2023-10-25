#include "GetRentsPrep.h"

#include "context/ApiGatewayContext.h"
#include <logger/LoggerFactory.h>

void GetRentsPrep::Init(const IRequestHandlerContextPtr &context)
{
    ApiGatewayContextPtr apiGatewayContext = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    apiGatewayContext->SetRequestType(ApiGatewayContext::GetRents);
}

void GetRentsPrep::SetRequestParameters(const std::vector<std::string> &)
{
}

void GetRentsPrep::ProcessRequest(const IRequestPtr &, std::string &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPrep::ProcessRequest unexpected call");
}

IClientServerRoute::ResponceType GetRentsPrep::ProcessResponse(const IResponsePtr &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPrep::ProcessResponse unexpected call");

    return IClientServerRoute::END_ROUTE;
}
