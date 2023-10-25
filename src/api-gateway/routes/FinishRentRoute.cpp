#include "FinishRentRoute.h"

#include <exceptions/server_exceptions.h>
#include <logger/LoggerFactory.h>

#include "clients.h"

void FinishRentRoute::Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t> clients)
{
    m_context = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    if (!m_context)
        throw ContextPtrCastException("finish rent route");

    m_clientsIndexes = clients;
}

void FinishRentRoute::SetRequestParameters(const std::vector<std::string> &)
{
}

void FinishRentRoute::ProcessRequest(const IRequestPtr &request, size_t &clientIndex)
{
    clientIndex = m_clientsIndexes[RENTS_CLIENT];

    std::string rentUid;
    if (m_context->GetRequestType() == ApiGatewayContext::FinishRent)
        rentUid = m_context->GetProcessInfo().finishRentRequest.rentUid;

    request->SetHeaders(m_context->GetCurrentRequest()->GetHeaders());
    request->SetMethod(net::POST);
    request->SetTarget(GET_RENT_BASE_TARGET + "/" + rentUid + "/finish");
}

IClientServerRoute::ResponceType FinishRentRoute::ProcessResponse(const IResponsePtr &responseFromClient)
{
    if (responseFromClient->GetStatus() == net::CODE_404)
    {
        m_context->GetCurrentResponse()->SetStatus(net::CODE_404);
        return IClientServerRoute::END_ROUTE;
    }

    if (responseFromClient->GetStatus() != net::CODE_200)
    {
        m_context->GetCurrentResponse()->copy(responseFromClient);
        throw ProcessingResponseException("finish rent route invalid code");
    }

    m_context->GetCurrentResponse()->SetStatus(net::CODE_204);

    return IClientServerRoute::END_ROUTE;
}
