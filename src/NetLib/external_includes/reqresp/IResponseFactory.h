#pragma once

#include "reqresp.h"

class IResponseFactory
{
public:
    virtual IResponsePtr CreateRespone() const = 0;
    virtual IResponsePtr CreateRespone(net::http_code_t, const std::string &body, const headers_t &headers) const;

    virtual ~IResponseFactory() = 0;
};

using IResponseFactoryPtr = std::shared_ptr<IResponseFactory>;