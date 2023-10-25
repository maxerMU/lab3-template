#ifndef BEASTRESP_H
#define BEASTRESP_H

#include <network/net_asio.h>
#include <reqresp/reqresp.h>

class BeastResp : public IResponse
{
public:
    BeastResp() = default;
    BeastResp(const http::response<http::string_body> &resp);
    std::string GetBody() const override;
    void SetBody(const std::string &body) override;

    headers_t GetHeaders() const override;
    void SetHeaders(const headers_t &headers) override;

    net::http_code_t GetStatus() const override;
    void SetStatus(net::http_code_t status) override;

private:
    std::string m_body;
    headers_t m_headers;
    net::http_code_t m_status = net::CODE_200;

    http::response<http::string_body> m_resp;

    bool m_isFromResp = false;
};

http::response<http::string_body> MakeBeastResp(const std::shared_ptr<IResponse> &resp);

#endif // BEASTRESP_H
