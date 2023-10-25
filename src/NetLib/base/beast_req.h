#ifndef BEASTREQ_H
#define BEASTREQ_H

#include <unordered_map>

#include <network/net_asio.h>
#include <reqresp/reqresp.h>

class BeastReq : public IRequest
{
public:
    BeastReq() = default;
    BeastReq(const http::request<http::string_body> &req);
    virtual std::string GetBody() const override;
    virtual std::string GetTarget() const override;
    virtual headers_t GetHeaders() const override;
    virtual net::method_t GetMethod() const override;

    virtual void SetBody(const std::string &body) override;
    virtual void SetHeaders(const headers_t &headers) override;
    virtual void SetTarget(const std::string &target) override;
    virtual void SetMethod(const net::method_t &method) override;

private:
    http::request<http::string_body> m_req;
    bool m_isFromReq = false;
    const std::unordered_map<std::string, net::method_t> m_methodsFrom{
        {"GET", net::GET}, {"POST", net::POST}, {"DELETE", net::DELETE}, {"PUT", net::PUT}, {"PATCH", net::PATCH}};

    std::string m_body;
    headers_t m_headers;
    std::string m_target;
    net::method_t m_method;
};

http::request<http::string_body> MakeBeastReq(const std::shared_ptr<IRequest> &req);

#endif // BEASTREQ_H
