#pragma once

#include <reqresp/reqresp.h>

class IRequestHandlerContext
{
public:
    virtual ~IRequestHandlerContext() = default;
    virtual IRequestPtr GetCurrentRequest() = 0;
    virtual IResponsePtr GetCurrentResponse() = 0;
};

using IRequestHandlerContextPtr = std::shared_ptr<IRequestHandlerContext>;
