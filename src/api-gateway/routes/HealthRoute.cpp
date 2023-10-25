#include "HealthRoute.h"

#include "context/ApiGatewayContext.h"
#include <logger/LoggerFactory.h>

void HealthRoute::Init(const IRequestHandlerContextPtr &context)
{
    context->GetCurrentResponse()->SetStatus(net::CODE_200);
}

void HealthRoute::SetRequestParameters(const std::vector<std::string> &)
{
}

void HealthRoute::ProcessRequest(const IRequestPtr &, std::string &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPrep::ProcessRequest unexpected call");
}

IClientServerRoute::ResponceType HealthRoute::ProcessResponse(const IResponsePtr &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPrep::ProcessResponse unexpected call");

    return IClientServerRoute::END_ROUTE;
}
