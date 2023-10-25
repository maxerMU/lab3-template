#include "BeastResponseFactory.h"

#include "beast_resp.h"

IResponsePtr BeastResponseFactory::CreateRespone() const
{
    return std::make_shared<BeastResp>();
}