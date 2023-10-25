#include "ApiGatewayHandler.h"

#include <logger/LoggerFactory.h>
#include <network/net.h>
#include <router/router.h>

#include "context/ApiGatewayContext.h"

ApiGatewayHandler::ApiGatewayHandler(const IConfigPtr &config) : m_config(config)
{
    SetClientIndexes(m_config);
}

IClientServerReqHandler::state_t ApiGatewayHandler::HandleRequest(const std::shared_ptr<IRequest> &request)
{
    LoggerFactory::GetLogger()->LogInfo((std::string("[gateway]: get request: ") + request->GetTarget()).c_str());
    if (!request->GetBody().empty())
        LoggerFactory::GetLogger()->LogInfo((std::string("[gateway]: request body: ") + request->GetBody()).c_str());

    m_context = std::make_shared<ApiGatewayContext>();
    m_context->GetCurrentRequest()->copy(request);

    m_routes = RequestsRouter::Instanse()->RouteClientServerReq(request->GetTarget(), request->GetMethod());
    if (m_routes.empty())
    {
        m_context->GetCurrentResponse()->SetStatus(net::CODE_404);
        m_context->GetCurrentResponse()->SetBody("not found");
        return RES_END;
    }

    m_currentRoute = 0;

    return RES_CONTINUE;
}

IClientServerReqHandler::state_t ApiGatewayHandler::GetNextRequest(IRequestPtr &request, size_t &clientIndex)
{
    if (m_currentRoute >= m_routes.size())
        return RES_END;

    try
    {
        if (m_routes[m_currentRoute]->GetRouteType() == IClientServerRoute::REQUEST_PREPARE)
            m_routes[m_currentRoute++]->Init(m_context, m_clientIndexes);

        m_routes[m_currentRoute]->Init(m_context, m_clientIndexes);
        
        if (m_routes[m_currentRoute]->GetRouteType() == IClientServerRoute::RESPONSE_MAKER)
            return RES_END;

        m_routes[m_currentRoute]->ProcessRequest(request, clientIndex);
    }
    catch (std::exception &ex)
    {
        LoggerFactory::GetLogger()->LogError((std::string("processing error: ") + ex.what()).c_str());
        m_context->GetCurrentResponse()->SetBody(ex.what());
        m_context->GetCurrentResponse()->SetStatus(net::CODE_503);
        return RES_END;
    }

    return RES_CONTINUE;
}

IClientServerReqHandler::state_t ApiGatewayHandler::HandleResponse(const IResponsePtr &response)
{
    try
    {
        IClientServerRoute::ResponceType respType = m_routes[m_currentRoute]->ProcessResponse(response);
        if (respType == IClientServerRoute::END_ROUTE)
            m_currentRoute++;

    }
    catch (std::exception &ex)
    {
        LoggerFactory::GetLogger()->LogError((std::string("processing resp: ") + ex.what()).c_str());
        m_context->GetCurrentResponse()->SetBody(ex.what());
        m_context->GetCurrentResponse()->SetStatus(net::CODE_503);
        return RES_END;
    }
    if (m_context->GetCurrentResponse()->GetStatus() != net::CODE_200)
        return RES_END;

    if (m_currentRoute == m_routes.size())
        return RES_END;

    return RES_CONTINUE;
}

void ApiGatewayHandler::MakeResponse(const IResponsePtr &resp)
{
    resp->copy(m_context->GetCurrentResponse());

    LoggerFactory::GetLogger()->LogInfo(
        (std::string("[gateway]: sending response status: ") + std::to_string((int)resp->GetStatus())).c_str());
    if (!resp->GetBody().empty())
        LoggerFactory::GetLogger()->LogInfo(
            (std::string("[gateway]: sending response body: ") + resp->GetBody()).c_str());
}
