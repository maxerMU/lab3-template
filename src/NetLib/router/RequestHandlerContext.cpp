#include "RequestHandlerContext.h"

#include "beast_req.h"
#include "beast_resp.h"

RequestsHandlerContext::RequestsHandlerContext()
{
    m_request = std::make_shared<BeastReq>();
    m_response = std::make_shared<BeastResp>();
}

IRequestPtr RequestsHandlerContext::GetCurrentRequest()
{
    return m_request;
}

IResponsePtr RequestsHandlerContext::GetCurrentResponse()
{
    return m_response;
}
