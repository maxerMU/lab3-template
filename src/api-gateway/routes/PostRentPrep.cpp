#include "PostRentPrep.h"

#include "context/ApiGatewayContext.h"
#include <logger/LoggerFactory.h>

void PostRentPrep::Init(const IRequestHandlerContextPtr &context)
{
    ApiGatewayContextPtr apiGatewayContext = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    apiGatewayContext->SetRequestType(ApiGatewayContext::PostRent);
    apiGatewayContext->GetProcessInfo().postRentRequest.postRent.FromJSON(context->GetCurrentRequest()->GetBody());
}

void PostRentPrep::SetRequestParameters(const std::vector<std::string> &)
{
}

void PostRentPrep::ProcessRequest(const IRequestPtr &, std::string &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPrep::ProcessRequest unexpected call");
}

IClientServerRoute::ResponceType PostRentPrep::ProcessResponse(const IResponsePtr &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPrep::ProcessResponse unexpected call");

    return IClientServerRoute::END_ROUTE;
}
