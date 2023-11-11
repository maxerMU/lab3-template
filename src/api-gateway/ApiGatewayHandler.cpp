#include "ApiGatewayHandler.h"

#include <logger/LoggerFactory.h>
#include <network/net.h>
#include <router/router.h>

#include "context/ApiGatewayContext.h"
#include "dto/ErrorMessageDTO.h"

ApiGatewayHandler::ApiGatewayHandler(const IConfigPtr &config) : m_config(config)
{
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

IClientServerReqHandler::state_t ApiGatewayHandler::ProcessError()
{
    LoggerFactory::GetLogger()->LogError("api gateway processing error: ");
    
    IRequestPtr tempreq = CreateRequestsHandlerContext()->GetCurrentRequest();
    std::string tempName;
    IClientServerRoute::RollbackType rollbackType = m_routes[m_currentRoute]->Rollback(tempreq, tempName);

    if (rollbackType == IClientServerRoute::NON_CRITICAL)
    {
        LoggerFactory::GetLogger()->LogInfo("non critical rollback. continue");
        if (m_currentRoute++ >= (long) m_routes.size())
            return RES_END;
    }
    else
    {
        m_isRollback = true;
        std::ostringstream oss;
        oss << m_lastClientName << " Service unavailable";
        ErrorMessageDTO errorDTO{oss.str()};
        m_context->GetCurrentResponse()->SetBody(errorDTO.ToJSON());
        m_context->GetCurrentResponse()->SetStatus(net::CODE_503);
        if (rollbackType == IClientServerRoute::NO_REQUEST)
        {
            m_currentRoute--;
        }
        LoggerFactory::GetLogger()->LogInfo("making rollback");
    }

    return RES_CONTINUE;
}

IClientServerReqHandler::state_t ApiGatewayHandler::GetNextRequest(IRequestPtr &request, std::string& clientName)
{
    if (m_currentRoute >= (long) m_routes.size())
        return RES_END;

    try
    {
        if (!m_isRollback)
        {
            if (m_routes[m_currentRoute]->GetRouteType() == IClientServerRoute::REQUEST_PREPARE)
                m_routes[m_currentRoute++]->Init(m_context);

            m_routes[m_currentRoute]->Init(m_context);

            if (m_routes[m_currentRoute]->GetRouteType() == IClientServerRoute::RESPONSE_MAKER)
                return RES_END;

            m_routes[m_currentRoute]->ProcessRequest(request, clientName);
        }
        else
        {
            bool needRequest = m_routes[m_currentRoute]->Rollback(request, clientName) == IClientServerRoute::NEED_REQUEST; 
            while (!needRequest && m_currentRoute > 0)
            {
                m_currentRoute--;
                needRequest = m_routes[m_currentRoute]->Rollback(request, clientName) == IClientServerRoute::NEED_REQUEST;
            }

            if (!needRequest)
                return RES_END;
        }
    }
    catch (std::exception &ex)
    {
        LoggerFactory::GetLogger()->LogError((std::string("processing error: ") + ex.what()).c_str());
        m_context->GetCurrentResponse()->SetBody(ex.what());
        m_context->GetCurrentResponse()->SetStatus(net::CODE_503);
        return RES_END;
    }

    m_lastClientName = clientName;

    return RES_CONTINUE;
}

IClientServerReqHandler::state_t ApiGatewayHandler::HandleResponse(const IResponsePtr &response)
{
    try
    {
        IClientServerRoute::ResponceType respType;
        if (m_isRollback)
            respType = m_routes[m_currentRoute]->ProcessRollbackResponse(response);
        else
            respType = m_routes[m_currentRoute]->ProcessResponse(response);
        if (respType == IClientServerRoute::END_ROUTE)
        {
            m_isRollback ? m_currentRoute-- : m_currentRoute++;
        }
    }
    catch (std::exception &ex)
    {
        LoggerFactory::GetLogger()->LogError((std::string("processing resp: ") + ex.what()).c_str());
        m_context->GetCurrentResponse()->SetBody(ex.what());
        m_context->GetCurrentResponse()->SetStatus(net::CODE_503);
        return RES_END;
    }

    if (m_context->GetCurrentResponse()->GetStatus() >= net::CODE_400)
        m_isRollback = true;

    if (m_currentRoute >= (long) m_routes.size() || m_currentRoute < 0)
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
