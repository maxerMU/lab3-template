#include "CarsServerHandler.h"

#include <logger/LoggerFactory.h>
#include <router/router.h>

CarsServerHandler::CarsServerHandler(const IResponseFactoryPtr &respFactory) : m_respFactory(respFactory)
{
}

void CarsServerHandler::HandleRequest(const std::shared_ptr<IRequest> &req)
{
    LoggerFactory::GetLogger()->LogInfo((std::string("[cars]: get request: ") + req->GetTarget()).c_str());
    if (!req->GetBody().empty())
        LoggerFactory::GetLogger()->LogInfo((std::string("[cars]: request body: ") + req->GetBody()).c_str());

    static_req_handler_t route = RequestsRouter::Instanse()->RouteReq(req->GetTarget(), req->GetMethod());

    m_resp = m_respFactory->CreateRespone();
    if (route)
    {
        try
        {
            route(m_resp, req);
        }
        catch (const std::exception &e)
        {
            LoggerFactory::GetLogger()->LogError((std::string("[cars]: unhandled exception: ") + e.what()).c_str());
            m_resp->SetStatus(net::CODE_503);
            m_resp->SetBody("Server error");
        }
    }
    else
    {
        m_resp->SetStatus(net::CODE_404);
        m_resp->SetBody("Not found");
    }
}

void CarsServerHandler::MakeResponse(const std::shared_ptr<IResponse> &resp)
{
    resp->copy(m_resp);
    LoggerFactory::GetLogger()->LogInfo(
        (std::string("[cars]: sending response status: ") + std::to_string((int)resp->GetStatus())).c_str());
    if (!resp->GetBody().empty())
        LoggerFactory::GetLogger()->LogInfo(
            (std::string("[cars]: sending response body: ") + resp->GetBody()).c_str());
}
