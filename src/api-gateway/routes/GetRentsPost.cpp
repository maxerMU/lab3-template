#include "GetRentsPost.h"

#include <logger/LoggerFactory.h>

#include "context/ApiGatewayContext.h"
#include "dto/GetRentDTO.h"

void GetRentsPost::Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t>)
{
    ApiGatewayContextPtr apiGatewayContext = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    ApiGatewayContext::RequestProcessInfo::GetRentsRequest rents = apiGatewayContext->GetProcessInfo().getRentsRequest;

    GetRentsDTO getRentsDTO;
    for (size_t i = 0; i < rents.rents.size(); ++i)
    {
        GetRentDTO getRentDto = {
            rents.rents[i].rentUid,
            rents.rents[i].status,
            rents.rents[i].dateFrom,
            rents.rents[i].dateTo,
            {
                rents.cars[i].carUid,
                rents.cars[i].brand,
                rents.cars[i].model,
                rents.cars[i].registrationNumber
            },
            {
                rents.payments[i].paymentUid,
                rents.payments[i].status,
                rents.payments[i].price,
            }
        };

        getRentsDTO.push_back(getRentDto);
    }

    apiGatewayContext->GetCurrentResponse()->SetBody(ToJSON(getRentsDTO));
    apiGatewayContext->GetCurrentResponse()->SetStatus(net::CODE_200);
}

void GetRentsPost::SetRequestParameters(const std::vector<std::string> &)
{
}

void GetRentsPost::ProcessRequest(const IRequestPtr &, size_t &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPost::ProcessRequest unexpected call");
}

IClientServerRoute::ResponceType GetRentsPost::ProcessResponse(const IResponsePtr &)
{
    LoggerFactory::GetLogger()->LogError("GetRentPost::ProcessResponse unexpected call");
    return IClientServerRoute::END_ROUTE;
}
