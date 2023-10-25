#include "beast_resp.h"

#define RESP_VERSION 11

BeastResp::BeastResp(const http::response<http::string_body> &resp) : m_resp(resp), m_isFromResp(true)
{
}

std::string BeastResp::GetBody() const
{
    if (m_isFromResp)
        return m_resp.body();

    return m_body;
}

headers_t BeastResp::GetHeaders() const
{
    if (!m_isFromResp)
        return m_headers;

    headers_t res;

    for (auto const &field : m_resp)
        res[std::string(field.name_string().data())] = std::string(field.value().data());

    return res;
}

net::http_code_t BeastResp::GetStatus() const
{
    if (!m_isFromResp)
        return m_status;

    return static_cast<net::http_code_t>(m_resp.result_int());
}

void BeastResp::SetBody(const std::string &body)
{
    if (!m_isFromResp)
    {
        m_body = body;
    }
    else
    {
        m_resp.body() = body;
    }
}

void BeastResp::SetHeaders(const headers_t &headers)
{
    if (!m_isFromResp)
    {
        m_headers = headers;
    }
    else
    {
        for (auto header : headers)
        {
            m_resp.set(header.first, header.second);
        }
    }
}

void BeastResp::SetStatus(net::http_code_t status)
{
    m_status = status;
}

http::response<http::string_body> MakeBeastResp(const std::shared_ptr<IResponse> &resp)
{
    http::response<http::string_body> res(http::status(resp->GetStatus()), RESP_VERSION);
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    // TODO подумать, как это влияет на работу с сокетами в сессии
    res.keep_alive(true);
    res.body() = std::string{resp->GetBody()};
    res.prepare_payload();

    return res;
}
