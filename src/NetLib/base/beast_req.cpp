#include "beast_req.h"

#define REQ_VERSION 11

http::request<http::string_body> MakeBeastReq(const std::shared_ptr<IRequest> &req)
{
    // TODO
    std::unordered_map<net::method_t, http::verb> methods_to{{net::GET, http::verb::get},
                                                             {net::POST, http::verb::post},
                                                             {net::DELETE, http::verb::delete_},
                                                             {net::PUT, http::verb::put},
                                                             {net::PATCH, http::verb::patch}};

    http::verb verb = http::verb::unknown;
    if (methods_to.contains(req->GetMethod()))
    {
        verb = methods_to[req->GetMethod()];
    }

    http::request<http::string_body> res(verb, req->GetTarget(), REQ_VERSION);

    for (auto header : req->GetHeaders())
    {
        res.set(header.first, header.second);
    }

    res.body() = req->GetBody();

    res.prepare_payload();

    return res;
}

BeastReq::BeastReq(const http::request<http::string_body> &req) : m_req(req), m_isFromReq(true)
{
}

std::string BeastReq::GetBody() const
{
    if (m_isFromReq)
        return m_req.body();

    return m_body;
}

std::string BeastReq::GetTarget() const
{
    if (!m_isFromReq)
        return m_target;
    std::string res(m_req.target());
    return res;
}

headers_t BeastReq::GetHeaders() const
{
    if (!m_isFromReq)
        return m_headers;

    headers_t res;

    for (auto const &field : m_req)
        res[std::string(field.name_string())] = std::string(field.value());

    return res;
}

net::method_t BeastReq::GetMethod() const
{
    if (!m_isFromReq)
        return m_method;

    std::string method(m_req.method_string());

    auto it = m_methodsFrom.find(method);
    if (it != m_methodsFrom.end())
        return it->second;

    return net::UNDEFINED;
}

void BeastReq::SetBody(const std::string &body)
{
    m_body = body;
}

void BeastReq::SetHeaders(const headers_t &headers)
{
    m_headers = headers;
}

void BeastReq::SetMethod(const net::method_t &method)
{
    m_method = method;
}

void BeastReq::SetTarget(const std::string &target)
{
    m_target = target;
}
