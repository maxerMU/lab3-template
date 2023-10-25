#include "GetCarsRoute.h"

#include "clients.h"
#include <logger/LoggerFactory.h>

void GetCarsRoute::Init(const IRequestHandlerContextPtr &context, const std::map<std::string, size_t> clients)
{
    m_context = context;
    m_clientsIndexes = clients;
}

void GetCarsRoute::SetRequestParameters(const std::vector<std::string> &params)
{
    if (params.size() != 3)
        LoggerFactory::GetLogger()->LogError("get cars incorrect params count");
}

void GetCarsRoute::ProcessRequest(const IRequestPtr &request, size_t &clientIndex)
{
    clientIndex = m_clientsIndexes[CARS_CLIENT];
    request->copy(m_context->GetCurrentRequest());
}

IClientServerRoute::ResponceType GetCarsRoute::ProcessResponse(const IResponsePtr &responseFromClient)
{
    m_context->GetCurrentResponse()->copy(responseFromClient);

    return IClientServerRoute::END_ROUTE;
}
