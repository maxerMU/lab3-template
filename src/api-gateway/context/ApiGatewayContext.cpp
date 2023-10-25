#include "ApiGatewayContext.h"

#include <network/net.h>


ApiGatewayContext::ApiGatewayContext()
{
    m_context = CreateRequestsHandlerContext();
}

IRequestPtr ApiGatewayContext::GetCurrentRequest()
{
    return m_context->GetCurrentRequest();
}

IResponsePtr ApiGatewayContext::GetCurrentResponse()
{
    return m_context->GetCurrentResponse();
}

ApiGatewayContext::RequestType ApiGatewayContext::GetRequestType() const
{
    return m_requestType;
}

void ApiGatewayContext::SetRequestType(const RequestType &reqType)
{
    m_requestType = reqType;
}

ApiGatewayContext::RequestProcessInfo& ApiGatewayContext::GetProcessInfo()
{
    return m_requestProcessInfo;
}
