#pragma once

#include <reqresp/IResponseFactory.h>

class BeastResponseFactory : public IResponseFactory
{
public:
    virtual IResponsePtr CreateRespone() const override;

    virtual ~BeastResponseFactory() = default;
};