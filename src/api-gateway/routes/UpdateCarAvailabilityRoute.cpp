#include "UpdateCarAvailabilityRoute.h"

#include <exceptions/logic_exceptions.h>
#include <exceptions/server_exceptions.h>
#include <logger/LoggerFactory.h>

#include "clients.h"
#include "dto/CarAvailabilityDTO.h"

void UpdateCarAvailabilityRoute::Init(const IRequestHandlerContextPtr &context)
{
    m_context = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    if (!m_context)
        throw ContextPtrCastException("update car route");
}

void UpdateCarAvailabilityRoute::SetRequestParameters(const std::vector<std::string> &)
{
}

void UpdateCarAvailabilityRoute::ProcessRequest(const IRequestPtr &request, std::string &clientName)
{
    clientName = CARS_CLIENT;

    std::string carUid;
    bool available = false;
    if (m_context->GetRequestType() == ApiGatewayContext::PostRent)
    {
        carUid = m_context->GetProcessInfo().postRentRequest.car.carUid;
        available = false;
    }
    else if (m_context->GetRequestType() == ApiGatewayContext::FinishRent)
    {
        carUid = m_context->GetProcessInfo().finishRentRequest.rent.carUid;
        available = true;
    }
    else if (m_context->GetRequestType() == ApiGatewayContext::CancelRent)
    {
        carUid = m_context->GetProcessInfo().cancelRentRequest.rent.carUid;
        available = true;
    }

    if (carUid.empty())
        throw UndefinedCarUidException("get car route");

    m_postedCar.carUid = carUid;
    m_postedCar.available = available;

    request->SetMethod(net::PATCH);
    request->SetBody(m_postedCar.ToJSON());
    request->SetTarget(GET_CAR_BASE_TARGET);
}

bool UpdateCarAvailabilityRoute::Rollback(const IRequestPtr &request, std::string &clientName)
{
    LoggerFactory::GetLogger()->LogInfo("ROLLBACK Update Car Availability");

    if (!m_isPostSuccessfull)
    {
        LoggerFactory::GetLogger()->LogWarning("update car available wasn't successfull");
        return false;
    }

    clientName = CARS_CLIENT;
    m_postedCar.available = !m_postedCar.available;

    request->SetMethod(net::PATCH);
    request->SetBody(m_postedCar.ToJSON());
    request->SetTarget(GET_CAR_BASE_TARGET);

    return true;
}

IClientServerRoute::ResponceType UpdateCarAvailabilityRoute::ProcessResponse(const IResponsePtr &responseFromClient)
{
    if (responseFromClient->GetStatus() == net::CODE_404)
    {
        m_context->GetCurrentResponse()->SetStatus(net::CODE_404);
        return IClientServerRoute::END_ROUTE;
    }

    if (responseFromClient->GetStatus() != net::CODE_200)
    {
        m_context->GetCurrentResponse()->copy(responseFromClient);
        throw ProcessingResponseException("get car route invalid code");
    }

    m_isPostSuccessfull = true;

    return IClientServerRoute::END_ROUTE;
}

IClientServerRoute::ResponceType UpdateCarAvailabilityRoute::ProcessRollbackResponse(
    const IResponsePtr &responseFromClient)
{
    if (responseFromClient->GetStatus() >= net::CODE_400)
        LoggerFactory::GetLogger()->LogError(
            (std::string("Failed rollback update car available: ") + m_postedCar.carUid).c_str());

    return IClientServerRoute::END_ROUTE;
}
