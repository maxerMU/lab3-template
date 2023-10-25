#include "PostRentPost.h"

#include <logger/LoggerFactory.h>

#include "context/ApiGatewayContext.h"
#include "dto/PostRentResultDTO.h"

void PostRentPost::Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t>)
{
    ApiGatewayContextPtr apiGatewayContext = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    ApiGatewayContext::RequestProcessInfo::PostRentRequest rent = apiGatewayContext->GetProcessInfo().postRentRequest;

    PostRentResultDTO postRentDto = {
        rent.rent.rentUid,
        rent.rent.status,
        rent.car.carUid,
        rent.rent.dateFrom,
        rent.rent.dateTo,
        {
            rent.payment.paymentUid,
            rent.payment.status,
            rent.payment.price,
        }
    };

    apiGatewayContext->GetCurrentResponse()->SetBody(postRentDto.ToJSON());
    apiGatewayContext->GetCurrentResponse()->SetStatus(net::CODE_200);
}

void PostRentPost::SetRequestParameters(const std::vector<std::string> &)
{
}

void PostRentPost::ProcessRequest(const IRequestPtr &, size_t &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPost::ProcessRequest unexpected call");
}

IClientServerRoute::ResponceType PostRentPost::ProcessResponse(const IResponsePtr &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPost::ProcessResponse unexpected call");
    return IClientServerRoute::END_ROUTE;
}
