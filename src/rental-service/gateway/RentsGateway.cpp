#include "RentsGateway.h"

#include "bl/RentsFacade.h"
#include <exceptions/logic_exceptions.h>
#include <logger/LoggerFactory.h>
#include <router/router.h>

void Health(const IResponsePtr &resp, const IRequestPtr &)
{
    resp->SetStatus(net::CODE_200);
}

std::string GetUsername(const IRequestPtr &request)
{
    headers_t headers = request->GetHeaders();
    auto it = headers.find("X-User-Name");
    if (it != headers.end())
    {
        return it->second;
    }

    return {};
}

void GetRents(const IResponsePtr &resp, const IRequestPtr &request)
{
    std::string username = GetUsername(request);
    if (username.empty())
    {
        LoggerFactory::GetLogger()->LogError("header X-User-Name not found");
        resp->SetStatus(net::CODE_400);
        return;
    }

    RentsDTO rents = RentsFacade::Instance()->GetRents(username);
    resp->SetBody(ToJSON(rents));
    resp->SetStatus(net::CODE_200);
}

void GetRent(const IResponsePtr &resp, const IRequestPtr &request, const std::vector<std::string>& params)
{
    if (params.size() != 1)
    {
        LoggerFactory::GetLogger()->LogError("Get rent incorrect params number");
        resp->SetStatus(net::CODE_400);
        return;
    }

    std::string username = GetUsername(request);
    if (username.empty())
    {
        LoggerFactory::GetLogger()->LogError("header X-User-Name not found");
        resp->SetStatus(net::CODE_400);
        return;
    }

    try
    {
        RentDTO rent = RentsFacade::Instance()->GetRent(username, params[0]);
        resp->SetBody(rent.ToJSON());
        resp->SetStatus(net::CODE_200);
    }
    catch(const RentNotFoundException&)
    {
        resp->SetStatus(net::CODE_404);
    }
}

void AddRent(const IResponsePtr &resp, const IRequestPtr &request)
{
    std::string username = GetUsername(request);
    if (username.empty())
    {
        LoggerFactory::GetLogger()->LogError("header X-User-Name not found");
        resp->SetStatus(net::CODE_400);
        return;
    }

    PostRentDTO postRent;
    postRent.FromJSON(request->GetBody());
    RentDTO rent = RentsFacade::Instance()->AddRent(postRent, username);

    resp->SetBody(rent.ToJSON());
    resp->SetStatus(net::CODE_200);
}

void FinishRent(const IResponsePtr &resp, const IRequestPtr &request, const std::vector<std::string>& params)
{
    if (params.size() != 1)
    {
        LoggerFactory::GetLogger()->LogError("Get rent incorrect params number");
        resp->SetStatus(net::CODE_400);
        return;
    }

    std::string username = GetUsername(request);
    if (username.empty())
    {
        LoggerFactory::GetLogger()->LogError("header X-User-Name not found");
        resp->SetStatus(net::CODE_400);
        return;
    }

    try
    {
        RentsFacade::Instance()->FinishRent(username, params[0]);
        resp->SetStatus(net::CODE_200);
    }
    catch(const RentNotFoundException&)
    {
        resp->SetStatus(net::CODE_404);
    }
}

void CancelRent(const IResponsePtr &resp, const IRequestPtr &request, const std::vector<std::string>& params)
{
    if (params.size() != 1)
    {
        LoggerFactory::GetLogger()->LogError("Get rent incorrect params number");
        resp->SetStatus(net::CODE_400);
        return;
    }

    std::string username = GetUsername(request);
    if (username.empty())
    {
        LoggerFactory::GetLogger()->LogError("header X-User-Name not found");
        resp->SetStatus(net::CODE_400);
        return;
    }

    try
    {
        RentsFacade::Instance()->CancelRent(username, params[0]);
        resp->SetStatus(net::CODE_200);
    }
    catch(const RentNotFoundException&)
    {
        resp->SetStatus(net::CODE_404);
    }
}

void SetupRouter()
{
    RequestsRouter::Instanse()->AddStaticEndpoint({"/manage/health", net::GET}, Health);
    RequestsRouter::Instanse()->AddStaticEndpoint({"/api/v1/rental", net::GET}, GetRents);
    RequestsRouter::Instanse()->AddDynamicEndpoint({std::regex("/api/v1/rental/([0-9\\-a-z]+)"), net::GET}, GetRent);
    RequestsRouter::Instanse()->AddStaticEndpoint({"/api/v1/rental", net::POST}, AddRent);
    RequestsRouter::Instanse()->AddDynamicEndpoint({std::regex("/api/v1/rental/([0-9\\-a-z]+)/finish"), net::POST}, FinishRent);
    RequestsRouter::Instanse()->AddDynamicEndpoint({std::regex("/api/v1/rental/([0-9\\-a-z]+)"), net::DELETE}, CancelRent);
}
