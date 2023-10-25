#include "PostRentRoute.h"

#include <boost/date_time/gregorian/gregorian.hpp>

#include <exceptions/server_exceptions.h>
#include <exceptions/logic_exceptions.h>
#include <logger/LoggerFactory.h>

#include "dto/PostRentDTO.h"

#include "clients.h"

void PostRentRoute::Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t> clients)
{
    m_context = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    if (!m_context)
        throw ContextPtrCastException("post rent route");

    m_clientsIndexes = clients;
}

void PostRentRoute::SetRequestParameters(const std::vector<std::string> &)
{
}

void PostRentRoute::ProcessRequest(const IRequestPtr &request, size_t &clientIndex)
{
    clientIndex = m_clientsIndexes[RENTS_CLIENT];

    std::string paymentUid, carUid, dateFrom, dateTo;
    if (m_context->GetRequestType() == ApiGatewayContext::PostRent)
    {
        paymentUid = m_context->GetProcessInfo().postRentRequest.payment.paymentUid;
        carUid = m_context->GetProcessInfo().postRentRequest.car.carUid;
        dateFrom = m_context->GetProcessInfo().postRentRequest.postRent.dateFrom;
        dateTo = m_context->GetProcessInfo().postRentRequest.postRent.dateTo;
    }

    PostRentDTO rent{paymentUid, carUid, dateFrom, dateTo};

    request->SetMethod(net::POST);
    request->SetHeaders(m_context->GetCurrentRequest()->GetHeaders());
    request->SetBody(rent.ToJSON());
    request->SetTarget(RENT_BASE_TARGET);
}

IClientServerRoute::ResponceType PostRentRoute::ProcessResponse(const IResponsePtr &responseFromClient)
{
    RentDTO rent;
    rent.FromJSON(responseFromClient->GetBody());

    if (m_context->GetRequestType() == ApiGatewayContext::PostRent)
    {
        m_context->GetProcessInfo().postRentRequest.rent = rent;
        return IClientServerRoute::END_ROUTE;
    }

    return IClientServerRoute::END_ROUTE;
}