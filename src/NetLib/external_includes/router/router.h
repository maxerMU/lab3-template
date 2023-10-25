#ifndef DETAILSROUTER_H
#define DETAILSROUTER_H

#include <functional>
#include <map>
#include <memory>
#include <regex>
#include <vector>

#include <reqresp/reqresp.h>
#include <router/IClientServerRoute.h>

struct RequestParams
{
    std::string target;
    net::method_t method;

    bool operator<(const RequestParams &params) const
    {
        return target < params.target || (target == params.target && method < params.method);
    }
};

struct RequestParamsRegEx
{
    std::regex target;
    net::method_t method;
};

using static_req_handler_t = std::function<void(const IResponsePtr &resp, const IRequestPtr &req)>;
using dynamic_req_handler_t =
    std::function<void(const IResponsePtr &resp, const IRequestPtr &req, const std::vector<std::string> &params)>;

class RequestsRouter;

using RequestsRouterPtr = std::shared_ptr<RequestsRouter>;

class RequestsRouter
{
public:
    static RequestsRouterPtr Instanse()
    {
        static RequestsRouterPtr router = RequestsRouterPtr(new RequestsRouter);
        return router;
    }

    static_req_handler_t RouteReq(const std::string &target, const net::method_t &method);
    std::vector<IClientServerRoutePtr> RouteClientServerReq(const std::string &target, const net::method_t &method);

    void AddStaticEndpoint(const RequestParams &params, const static_req_handler_t &fun);
    void AddStaticEndpoint(const RequestParams &params, const std::vector<IClientServerRouteCreatorPtr>& routesCreators);

    void AddDynamicEndpoint(const RequestParamsRegEx &params, const dynamic_req_handler_t &fun);
    void AddDynamicEndpoint(const RequestParamsRegEx &params, std::vector<IClientServerRouteCreatorPtr>& routesCreators);

private:
    using dynamic_route_t = std::pair<RequestParamsRegEx, dynamic_req_handler_t>;
    using dynamic_routes_t = std::vector<dynamic_route_t>;
    using dynamic_client_server_route_t = std::pair<RequestParamsRegEx, std::vector<IClientServerRouteCreatorPtr>>;
    using dynamic_client_server_routes_t = std::vector<dynamic_client_server_route_t>;

    using static_routes_t = std::map<RequestParams, static_req_handler_t>;
    using static_client_server_routes_t = std::map<RequestParams, std::vector<IClientServerRouteCreatorPtr>>;

    static_routes_t m_staticRoutes;
    dynamic_routes_t m_dynamicRoutes;
    static_client_server_routes_t m_staticClientServerRoutesCreators;
    dynamic_client_server_routes_t m_dynamicClientServerRoutesCreators;

    RequestsRouter();
    RequestsRouter(RequestsRouter &) = delete;

    std::vector<std::string> GetDynRouteParams(const std::string &target, const std::regex &route);
    static_req_handler_t WrapDynamicRequest(const std::string &target, const dynamic_route_t &route);
};

#endif // DETAILSROUTER_H
