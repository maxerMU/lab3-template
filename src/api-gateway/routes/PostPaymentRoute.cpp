#include "PostPaymentRoute.h"

#include <boost/date_time/gregorian/gregorian.hpp>

#include <exceptions/logic_exceptions.h>
#include <exceptions/server_exceptions.h>
#include <logger/LoggerFactory.h>

#include "dto/PostPaymentDTO.h"

#include "clients.h"

void PostPaymentRoute::Init(const IRequestHandlerContextPtr &context)
{
    m_context = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    if (!m_context)
        throw ContextPtrCastException("post payment route");
}

void PostPaymentRoute::SetRequestParameters(const std::vector<std::string> &)
{
}

void PostPaymentRoute::ProcessRequest(const IRequestPtr &request, std::string &clientName)
{
    clientName = PAYMENTS_CLIENT;

    size_t price = CalculatePrice();

    request->SetMethod(net::POST);
    request->SetBody(PostPaymentDTO{price}.ToJSON());
    request->SetTarget(PAYMENT_BASE_TARGET);
}

IClientServerRoute::RollbackType PostPaymentRoute::Rollback(const IRequestPtr &request, std::string &clientName)
{
    LoggerFactory::GetLogger()->LogInfo("ROLLBACK Post Payment");

    if (m_postedPaymentUid.empty())
    {
        LoggerFactory::GetLogger()->LogWarning("postedPaymentUid is empty");
        return IClientServerRoute::SKIP;
    }

    clientName = PAYMENTS_CLIENT;

    request->SetMethod(net::DELETE);
    request->SetTarget(std::string(PAYMENT_BASE_TARGET) + "/" + m_postedPaymentUid);

    return IClientServerRoute::NEED_REQUEST;
}

IClientServerRoute::ResponceType PostPaymentRoute::ProcessResponse(const IResponsePtr &responseFromClient)
{
    PaymentDTO payment;
    payment.FromJSON(responseFromClient->GetBody());

    m_postedPaymentUid = payment.paymentUid;

    if (m_context->GetRequestType() == ApiGatewayContext::PostRent)
    {
        m_context->GetProcessInfo().postRentRequest.payment = payment;
        return IClientServerRoute::END_ROUTE;
    }

    return IClientServerRoute::END_ROUTE;
}

IClientServerRoute::ResponceType PostPaymentRoute::ProcessRollbackResponse(const IResponsePtr &responseFromClient)
{
    if (responseFromClient->GetStatus() >= net::CODE_400)
        LoggerFactory::GetLogger()->LogError((std::string("Failed rollback post payment: ") + m_postedPaymentUid).c_str());

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
