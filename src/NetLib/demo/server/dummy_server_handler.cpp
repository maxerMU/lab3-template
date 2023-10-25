#include "dummy_server_handler.h"

#include <router/router.h>

#include <iostream>

DummyServerHandler::DummyServerHandler(const IResponseFactoryPtr &respFactory) : m_respFactory(respFactory)
{
}

void DummyServerHandler::HandleRequest(const std::shared_ptr<IRequest> &req)
{
    std::cout << req->GetBody() << std::endl;
    std::cout << req->GetTarget() << std::endl;

    static_req_handler_t route = RequestsRouter::Instanse()->RouteReq(req->GetTarget(), req->GetMethod());

    if (route)
    {
        m_resp = m_respFactory->CreateRespone();
        route(m_resp, req);
    }
    else
    {
        m_resp->SetStatus(net::CODE_404);
        m_resp->SetBody("Not found");
    }
}

void DummyServerHandler::MakeResponse(const std::shared_ptr<IResponse> &resp)
{
    resp->copy(m_resp);
}
