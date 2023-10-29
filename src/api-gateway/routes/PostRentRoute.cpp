#include "PostRentRoute.h"

#include <boost/date_time/gregorian/gregorian.hpp>

#include <exceptions/logic_exceptions.h>
#include <exceptions/server_exceptions.h>
#include <logger/LoggerFactory.h>

#include "dto/PostRentDTO.h"

#include "clients.h"

void PostRentRoute::Init(const IRequestHandlerContextPtr &context)
{
    m_context = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    if (!m_context)
        throw ContextPtrCastException("post rent route");
}

void PostRentRoute::SetRequestParameters(const std::vector<std::string> &)
{
}

void PostRentRoute::ProcessRequest(const IRequestPtr &request, std::string &clientName)
{
    clientName = RENTS_CLIENT;

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

bool PostRentRoute::Rollback(const IRequestPtr &request, std::string &clientName)
{
    LoggerFactory::GetLogger()->LogInfo("ROLLBACK Post Rent");

    if (m_postedRentUid.empty())
    {
        LoggerFactory::GetLogger()->LogWarning("postedRentUid is empty");
        return false;
    }

    clientName = RENTS_CLIENT;

    request->SetMethod(net::DELETE);
    request->SetHeaders(m_context->GetCurrentRequest()->GetHeaders());
    request->SetTarget(std::string(RENT_BASE_TARGET) + "/" + m_postedRentUid);

    return true;
}

IClientServerRoute::ResponceType PostRentRoute::ProcessResponse(const IResponsePtr &responseFromClient)
{
    RentDTO rent;
    rent.FromJSON(responseFromClient->GetBody());

    m_postedRentUid = rent.rentUid;

    if (m_context->GetRequestType() == ApiGatewayContext::PostRent)
    {
        m_context->GetProcessInfo().postRentRequest.rent = rent;
        return IClientServerRoute::END_ROUTE;
    }

    return IClientServerRoute::END_ROUTE;
}

IClientServerRoute::ResponceType PostRentRoute::ProcessRollbackResponse(const IResponsePtr &responseFromClient)
{
    if (responseFromClient->GetStatus() >= net::CODE_400)
        LoggerFactory::GetLogger()->LogError((std::string("Failed rollback post rent: ") + m_postedRentUid).c_str());

    return IClientServerRoute::END_ROUTE;
}
