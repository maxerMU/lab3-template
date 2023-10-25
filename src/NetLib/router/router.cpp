#include <router/router.h>

#include <logger/LoggerFactory.h>

#include <iostream>
#include <router/router.h>

static_req_handler_t RequestsRouter::RouteReq(const std::string &target, const net::method_t &method)
{
    RequestParams static_params{target, method};
    static_req_handler_t creator;

    auto it = m_staticRoutes.find(static_params);
    if (it != m_staticRoutes.end())
        creator = it->second;

    if (!creator)
    {
        for (const auto &dynRoute : m_dynamicRoutes)
        {
            if (std::regex_match(target, dynRoute.first.target) && method == dynRoute.first.method)
            {
                creator = WrapDynamicRequest(target, dynRoute);
                break;
            }
        }
    }

    if (!creator)
    {
        LoggerFactory::GetLogger()->LogWarning(std::string("Wrong endpoint: " + target).c_str());
    }

    return creator;
}

std::vector<IClientServerRoutePtr> RequestsRouter::RouteClientServerReq(const std::string &target,
                                                                        const net::method_t &method)
{
    std::vector<IClientServerRoutePtr> routes;

    // static
    RequestParams staticParams{target, method};
    auto it = m_staticClientServerRoutesCreators.find(staticParams);
    if (it != m_staticClientServerRoutesCreators.end())
    {
        for (const auto& routeCreator : it->second)
            routes.push_back(routeCreator->Create());
    }

    if (!routes.empty())
        return routes;

    // regex
    for (const auto &dynRoutesCreators : m_dynamicClientServerRoutesCreators)
    {
        if (std::regex_match(target, dynRoutesCreators.first.target) && method == dynRoutesCreators.first.method)
        {
            const std::vector<std::string> params = GetDynRouteParams(target, dynRoutesCreators.first.target);
            for (const auto &routeCreator : dynRoutesCreators.second)
            {
                routes.push_back(routeCreator->Create());
                routes.back()->SetRequestParameters(params);
            }

            break;
        }
    }

    if (routes.empty())
        LoggerFactory::GetLogger()->LogWarning(std::string("Wrong endpoint: " + target).c_str());

    return routes;
}
void RequestsRouter::AddStaticEndpoint(const RequestParams &params, const static_req_handler_t &fun)
{
    m_staticRoutes[params] = fun;
}

void RequestsRouter::AddDynamicEndpoint(const RequestParamsRegEx &params, const dynamic_req_handler_t &fun)
{
    m_dynamicRoutes.push_back(dynamic_route_t(params, fun));
}

void RequestsRouter::AddStaticEndpoint(const RequestParams &params, const std::vector<IClientServerRouteCreatorPtr>& routesCreators)
{
    m_staticClientServerRoutesCreators[params] = routesCreators;
}

void RequestsRouter::AddDynamicEndpoint(const RequestParamsRegEx &params,
                                        std::vector<IClientServerRouteCreatorPtr> &routesCreators)
{
    m_dynamicClientServerRoutesCreators.push_back(dynamic_client_server_route_t(params, routesCreators));
}

RequestsRouter::RequestsRouter()
{
}

std::vector<std::string> RequestsRouter::GetDynRouteParams(const std::string &target, const std::regex &route)
{
    std::smatch match;
    std::regex_search(target, match, route);

    std::vector<std::string> params;
    for (size_t i = 1; i < match.size(); i++)
    {
        params.push_back(match[i].str());
    }

    return params;
}

static_req_handler_t RequestsRouter::WrapDynamicRequest(const std::string &target, const dynamic_route_t &route)
{
    std::vector<std::string> params = GetDynRouteParams(target, route.first.target);
    return std::bind(route.second, std::placeholders::_1, std::placeholders::_2, params);
}
