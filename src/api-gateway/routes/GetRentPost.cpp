#include "GetRentPost.h"

#include <logger/LoggerFactory.h>

#include "context/ApiGatewayContext.h"
#include "dto/GetRentDTO.h"

void GetRentPost::Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t>)
{
    ApiGatewayContextPtr apiGatewayContext = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    ApiGatewayContext::RequestProcessInfo::GetRentRequest rent = apiGatewayContext->GetProcessInfo().getRentRequest;

    GetRentDTO getRentDto = {
        rent.rent.rentUid,
        rent.rent.status,
        rent.rent.dateFrom,
        rent.rent.dateTo,
        {
            rent.car.carUid,
            rent.car.brand,
            rent.car.model,
            rent.car.registrationNumber
        },
        {
            rent.payment.paymentUid,
            rent.payment.status,
            rent.payment.price,
        }
    };

    apiGatewayContext->GetCurrentResponse()->SetBody(getRentDto.ToJSON());
    apiGatewayContext->GetCurrentResponse()->SetStatus(net::CODE_200);
}

void GetRentPost::SetRequestParameters(const std::vector<std::string> &)
{
}

void GetRentPost::ProcessRequest(const IRequestPtr &, size_t &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPost::ProcessRequest unexpected call");
}

IClientServerRoute::ResponceType GetRentPost::ProcessResponse(const IResponsePtr &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPost::ProcessResponse unexpected call");
    return IClientServerRoute::END_ROUTE;
}
