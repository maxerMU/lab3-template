#include "CancelRentRoute.h"

#include <exceptions/server_exceptions.h>
#include <logger/LoggerFactory.h>

#include "clients.h"

void CancelRentRoute::Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t> clients)
{
    m_context = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    if (!m_context)
        throw ContextPtrCastException("cancel rent route");

    m_clientsIndexes = clients;
}

void CancelRentRoute::SetRequestParameters(const std::vector<std::string> &)
{
}

void CancelRentRoute::ProcessRequest(const IRequestPtr &request, size_t &clientIndex)
{
    clientIndex = m_clientsIndexes[RENTS_CLIENT];

    std::string rentUid;
    if (m_context->GetRequestType() == ApiGatewayContext::CancelRent)
        rentUid = m_context->GetProcessInfo().cancelRentRequest.rentUid;

    request->SetHeaders(m_context->GetCurrentRequest()->GetHeaders());
    request->SetMethod(net::DELETE);
    request->SetTarget(GET_RENT_BASE_TARGET + "/" + rentUid);
}

IClientServerRoute::ResponceType CancelRentRoute::ProcessResponse(const IResponsePtr &responseFromClient)
{
    if (responseFromClient->GetStatus() == net::CODE_404)
    {
        m_context->GetCurrentResponse()->SetStatus(net::CODE_404);
        return IClientServerRoute::END_ROUTE;
    }

    if (responseFromClient->GetStatus() != net::CODE_200)
    {
        m_context->GetCurrentResponse()->copy(responseFromClient);
        throw ProcessingResponseException("cancel rent route invalid code");
    }

    m_context->GetCurrentResponse()->SetStatus(net::CODE_204);

    return IClientServerRoute::END_ROUTE;
}
