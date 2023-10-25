#include "HealthRoute.h"

#include <logger/LoggerFactory.h>
#include "context/ApiGatewayContext.h"

void HealthRoute::Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t>)
{
    context->GetCurrentResponse()->SetStatus(net::CODE_200);
}

void HealthRoute::SetRequestParameters(const std::vector<std::string> &)
{
}

void HealthRoute::ProcessRequest(const IRequestPtr &, size_t &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPrep::ProcessRequest unexpected call");
}

IClientServerRoute::ResponceType HealthRoute::ProcessResponse(const IResponsePtr &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPrep::ProcessResponse unexpected call");

    return IClientServerRoute::END_ROUTE;
}
