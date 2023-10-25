#include "GetRentsRoute.h"

#include "clients.h"
#include <logger/LoggerFactory.h>

void GetRentsRoute::Init(const IRequestHandlerContextPtr &context)
{
    m_context = std::dynamic_pointer_cast<ApiGatewayContext>(context);
    if (!m_context)
        throw ContextPtrCastException("get car route");
}

void GetRentsRoute::SetRequestParameters(const std::vector<std::string> &)
{
}

void GetRentsRoute::ProcessRequest(const IRequestPtr &request, std::string &clientName)
{
    clientName = RENTS_CLIENT;

    request->SetHeaders(m_context->GetCurrentRequest()->GetHeaders());
    request->SetTarget(GET_RENTS_BASE_TARGET);
}

IClientServerRoute::ResponceType GetRentsRoute::ProcessResponse(const IResponsePtr &responseFromClient)
{
    m_context->GetProcessInfo().getRentsRequest.rents = FromJSON(responseFromClient->GetBody());

    return IClientServerRoute::END_ROUTE;
}
