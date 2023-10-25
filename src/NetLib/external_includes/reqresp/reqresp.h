#ifndef REQRESP_H
#define REQRESP_H

#include <exceptions/server_exceptions.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

// TODO change to map
typedef std::map<std::string, std::string> headers_t;

#ifdef DELETE
#undef DELETE
#endif

namespace net
{
enum method_t
{
    GET,
    POST,
    DELETE,
    PUT,
    PATCH,
    UNDEFINED
};

enum http_code_t
{
    CODE_200 = 200, // OK
    CODE_201 = 201, // Created
    CODE_204 = 204, // No content
    CODE_400 = 400, // bad request
    CODE_401 = 401, // unauthorized
    CODE_403 = 403, // forbidden
    CODE_404 = 404, // not found
    CODE_503 = 503  // internal server error
};

} // namespace net

class IRequest;
using IRequestPtr = std::shared_ptr<IRequest>;

class IRequest
{
public:
    virtual ~IRequest() = default;

    virtual std::string GetBody() const = 0;
    virtual std::string GetTarget() const = 0;
    virtual headers_t GetHeaders() const = 0;
    virtual net::method_t GetMethod() const = 0;

    virtual void SetBody(const std::string &body) = 0;
    virtual void SetHeaders(const headers_t &headers) = 0;
    virtual void SetTarget(const std::string &target) = 0;
    virtual void SetMethod(const net::method_t &method) = 0;

    void copy(const IRequestPtr &another_req)
    {
        this->SetBody(another_req->GetBody());
        this->SetTarget(another_req->GetTarget());
        this->SetHeaders(another_req->GetHeaders());
        this->SetMethod(another_req->GetMethod());
    }
};

class IResponse;

using IResponsePtr = std::shared_ptr<IResponse>;

class IResponse
{
public:
    virtual ~IResponse() = default;
    virtual std::string GetBody() const = 0;
    virtual void SetBody(const std::string &body) = 0;

    virtual headers_t GetHeaders() const = 0;
    virtual void SetHeaders(const headers_t &headers) = 0;

    virtual net::http_code_t GetStatus() const = 0;
    virtual void SetStatus(net::http_code_t status) = 0;

    void copy(const IResponsePtr &another_resp)
    {
        this->SetBody(another_resp->GetBody());
        this->SetHeaders(another_resp->GetHeaders());
        this->SetStatus(another_resp->GetStatus());
    }
};

#endif // REQRESP_H
