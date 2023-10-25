#include "GetRentsPrep.h"

#include <logger/LoggerFactory.h>
#include "context/ApiGatewayContext.h"

void GetRentsPrep::Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t>)
{
    ApiGatewayContextPtr apiGatewayContext = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    apiGatewayContext->SetRequestType(ApiGatewayContext::GetRents);
}

void GetRentsPrep::SetRequestParameters(const std::vector<std::string> &)
{
}

void GetRentsPrep::ProcessRequest(const IRequestPtr &, size_t &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPrep::ProcessRequest unexpected call");
}

IClientServerRoute::ResponceType GetRentsPrep::ProcessResponse(const IResponsePtr &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPrep::ProcessResponse unexpected call");

    return IClientServerRoute::END_ROUTE;
}
