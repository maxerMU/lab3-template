#pragma once

#include <router/IRequestHandlerContext.h>

class RequestsHandlerContext
    : public IRequestHandlerContext
{
public:
    RequestsHandlerContext();
    virtual ~RequestsHandlerContext() = default;

    virtual IRequestPtr GetCurrentRequest() override;
    virtual IResponsePtr GetCurrentResponse() override;

private:
    IRequestPtr m_request;
    IResponsePtr m_response;
};