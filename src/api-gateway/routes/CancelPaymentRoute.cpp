#include "CancelPaymentRoute.h"

#include <boost/date_time/gregorian/gregorian.hpp>

#include <exceptions/server_exceptions.h>
#include <exceptions/logic_exceptions.h>
#include <logger/LoggerFactory.h>

#include "clients.h"

void CancelPaymentRoute::Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t> clients)
{
    m_context = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    if (!m_context)
        throw ContextPtrCastException("cancel payment route");

    m_clientsIndexes = clients;
}

void CancelPaymentRoute::SetRequestParameters(const std::vector<std::string> &)
{
}

void CancelPaymentRoute::ProcessRequest(const IRequestPtr &request, size_t &clientIndex)
{
    clientIndex = m_clientsIndexes[PAYMENTS_CLIENT];

    std::string paymentUid;
    if (m_context->GetRequestType() == ApiGatewayContext::CancelRent)
        paymentUid = m_context->GetProcessInfo().cancelRentRequest.rent.paymentUid;

    if (paymentUid.empty())
        throw UndefinedPaymentUidException("get payment route");

    request->SetMethod(net::DELETE);
    request->SetTarget(PAYMENT_BASE_TARGET + "/" + paymentUid);
}

IClientServerRoute::ResponceType CancelPaymentRoute::ProcessResponse(const IResponsePtr &responseFromClient)
{
    if (responseFromClient->GetStatus() == net::CODE_404)
    {
        m_context->GetCurrentResponse()->SetStatus(net::CODE_404);
        return IClientServerRoute::END_ROUTE;
    }

    if (responseFromClient->GetStatus() != net::CODE_200)
    {
        m_context->GetCurrentResponse()->copy(responseFromClient);
        throw ProcessingResponseException("cancel payment route invalid code");
    }

    return IClientServerRoute::END_ROUTE;
}