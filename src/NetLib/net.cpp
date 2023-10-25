#include <network/net.h>

#include "base/BeastResponseFactory.h"
#include "connections/server_connection.h"
#include "connections/client_server_connection.h"
#include "sessions/http_session.h"
#include "sessions/http_client_server_session.h"
#include "router/RequestHandlerContext.h"

IResponseFactoryPtr CreateResponceFactory()
{
    return std::make_shared<BeastResponseFactory>();
}

IServerSessionCreatorPtr CreateServerSessionCreator(const IServerReqHandlerCreatorPtr &handlerCreator)
{
    return std::make_shared<HttpServerSessionCreator>(handlerCreator);
}

IClientServerSessionCreatorPtr CreateClientServerSessionCreator(const IClientServerReqHandlerCreatorPtr &handlerCreator, const IConfigPtr &config)
{
    return std::make_shared<HttpClientServerSessionCreator>(handlerCreator, config);
}

IClientServerConnectionPtr CreateClientServerConnection(asio::io_context &context,
                                            const IClientServerSessionCreatorPtr &sessionCreator,
                                            const IConfigPtr &config)
{
    return std::make_shared<ClientServerConnection>(context, sessionCreator, config);
}

IRequestHandlerContextPtr CreateRequestsHandlerContext()
{
    return std::make_shared<RequestsHandlerContext>();
}

IServerConnectionPtr CreateServerConnection(asio::io_context &context, const IServerSessionCreatorPtr &creator,
                                      const IConfigPtr &config)
{
    return std::make_shared<ServerConnection>(context, creator, config);
}