#pragma once

#include <network/net_asio.h>
#include <config/IConfig.h>

#include <reqresp/IResponseFactory.h>
#include <request_handlers/IServerReqHandler.h>
#include <request_handlers/IClientServerReqHandler.h>
#include <network/IServerConnection.h>
#include <network/IClientServerConnection.h>
#include <network/IServerSession.h>
#include <network/IClientServerSession.h>

#include <router/IRequestHandlerContext.h>

IResponseFactoryPtr CreateResponceFactory();
IServerSessionCreatorPtr CreateServerSessionCreator(const IServerReqHandlerCreatorPtr &handlerCreator);
IServerConnectionPtr CreateServerConnection(asio::io_context &context, const std::shared_ptr<IServerSessionCreator> &creator, const IConfigPtr &config);

IClientServerSessionCreatorPtr CreateClientServerSessionCreator(const IClientServerReqHandlerCreatorPtr &handlerCreator, const IConfigPtr &config);
IClientServerConnectionPtr CreateClientServerConnection(asio::io_context &context, const IClientServerSessionCreatorPtr& sessionCreator, const IConfigPtr& config);
IClientServerConnectionPtr CreateCircuitBreakerClientServerConnection(asio::io_context &context, const IClientServerSessionCreatorPtr& sessionCreator, const IConfigPtr& config);
IRequestHandlerContextPtr CreateRequestsHandlerContext();