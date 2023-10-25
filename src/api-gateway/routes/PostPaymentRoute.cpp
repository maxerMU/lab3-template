#include "PostPaymentRoute.h"

#include <boost/date_time/gregorian/gregorian.hpp>

#include <exceptions/server_exceptions.h>
#include <exceptions/logic_exceptions.h>
#include <logger/LoggerFactory.h>

#include "dto/PostPaymentDTO.h"

#include "clients.h"

void PostPaymentRoute::Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t> clients)
{
    m_context = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    if (!m_context)
        throw ContextPtrCastException("post payment route");

    m_clientsIndexes = clients;
}

void PostPaymentRoute::SetRequestParameters(const std::vector<std::string> &)
{
}

void PostPaymentRoute::ProcessRequest(const IRequestPtr &request, size_t &clientIndex)
{
    clientIndex = m_clientsIndexes[PAYMENTS_CLIENT];

    size_t price = CalculatePrice();

    request->SetMethod(net::POST);
    request->SetBody(PostPaymentDTO{price}.ToJSON());
    request->SetTarget(GET_PAYMENT_BASE_TARGET);
}

IClientServerRoute::ResponceType PostPaymentRoute::ProcessResponse(const IResponsePtr &responseFromClient)
{
    PaymentDTO payment;
    payment.FromJSON(responseFromClient->GetBody());

    if (m_context->GetRequestType() == ApiGatewayContext::PostRent)
    {
        m_context->GetProcessInfo().postRentRequest.payment = payment;
        return IClientServerRoute::END_ROUTE;
    }

    return IClientServerRoute::END_ROUTE;
}

size_t PostPaymentRoute::CalculatePrice()
{
    std::string dateFrom, dateTo;
    size_t pricePerDay = 1;

    if (m_context->GetRequestType() == ApiGatewayContext::PostRent)
    {
        dateFrom = m_context->GetProcessInfo().postRentRequest.postRent.dateFrom;
        dateTo = m_context->GetProcessInfo().postRentRequest.postRent.dateTo;
        pricePerDay = m_context->GetProcessInfo().postRentRequest.car.price;
    }

    boost::gregorian::date dateStart(boost::gregorian::from_string(dateFrom));
    boost::gregorian::date dateEnd(boost::gregorian::from_string(dateTo));
    int days = (dateEnd - dateStart).days();
    if (days <= 0)
        throw IncorrectDatesException("days diff less than zero");
    
    return pricePerDay * days;
}
