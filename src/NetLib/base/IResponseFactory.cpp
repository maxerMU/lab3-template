#include <reqresp/IResponseFactory.h>

IResponsePtr IResponseFactory::CreateRespone(net::http_code_t code, const std::string &body,
                                             const headers_t &headers) const
{
    IResponsePtr resp = this->CreateRespone();

    resp->SetStatus(code);
    resp->SetBody(body);
    resp->SetHeaders(headers);

    return resp;
}

IResponseFactory::~IResponseFactory() = default;