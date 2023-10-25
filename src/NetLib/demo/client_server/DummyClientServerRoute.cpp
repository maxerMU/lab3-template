#include "DummyClientServerRoute.h"

#include <logger/LoggerFactory.h>
#include "clients.h"

void DummyClientServerRoute::Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t> clients)
{
    m_context = context;
    m_clientsIndexes = clients;
}

void DummyClientServerRoute::SetRequestParameters(const std::vector<std::string> &)
{
    LoggerFactory::GetLogger()->LogWarning("got params in static handler");
}

void DummyClientServerRoute::ProcessRequest(const IRequestPtr &request, size_t &clientIndex)
{
    clientIndex = m_clientsIndexes[DUMMY_CLIENT];
    request->SetTarget("/info");
    request->SetMethod(net::GET);
}

IClientServerRoute::ResponceType DummyClientServerRoute::ProcessResponse(const IResponsePtr &responseFromClient)
{
    m_context->GetCurrentResponse()->copy(responseFromClient);
    return IClientServerRoute::END_ROUTE;
}
