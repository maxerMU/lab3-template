#include "CircuitBreakerConnection.h"

#include <boost/bind.hpp>

#include <exceptions/server_exceptions.h>
#include <config/base_sections.h>
#include <logger/LoggerFactory.h>

CircuitBreakerConnection::CircuitBreakerConnection(asio::io_context &context,
                                                   const std::shared_ptr<IClientServerSessionCreator> &creator,
                                                   const std::shared_ptr<IConfig> &config)
    : ClientServerConnection(context, creator, config)
{
    ReadConfig(config);
}

std::shared_ptr<tcp::socket> CircuitBreakerConnection::ConnetClientSocket(const std::string &clientName)
{
    auto it = m_clientName2FailedRequests.find(clientName);
    if (it == m_clientName2FailedRequests.end())
        m_clientName2FailedRequests[clientName] = 0;
    else if (it->second >= m_maxRetryCount)
    {
        LoggerFactory::GetLogger()->LogError((std::string("Circuit breaker is opened for client: ") + clientName).c_str());
        throw ClientOpenedCircuitBreakerException(clientName);
    }
    
    try
    {
        return ClientServerConnection::ConnetClientSocket(clientName);
    }
    catch (...)
    {
        m_clientName2FailedRequests[clientName] += 1;
        if (m_clientName2FailedRequests[clientName] >= m_maxRetryCount)
            SetTimer(clientName);
        throw;
    }
}

void CircuitBreakerConnection::ReadConfig(const IConfigPtr &config)
{
    m_maxRetryCount = config->GetUintField({CIRCUIT_BREAKER_SECTION, CIRCUIT_BREAKER_RETRY_COUNT_SECTION});
    m_reconnectionTimeout =config->GetUintField({CIRCUIT_BREAKER_SECTION, CIRCUIT_BREAKER_RECONNECT_TIMEOUT_SECTION});
}

void CircuitBreakerConnection::SetTimer(const std::string &clientName)
{
    std::shared_ptr<asio::deadline_timer> timer;
    size_t timerIndex;
    if (!m_freeTimersIndexes.empty())
    {
        timerIndex = m_freeTimersIndexes.back();
        timer = m_timersPool[timerIndex];
        m_freeTimersIndexes.pop_back();
    }
    else
    {
        timer = std::make_shared<asio::deadline_timer>(m_context);
        timerIndex = m_timersPool.size();
        m_timersPool.push_back(timer);
    }

    LoggerFactory::GetLogger()->LogInfo((std::string("Set timer for client: ") + clientName).c_str());
    timer->expires_from_now(boost::posix_time::seconds(m_reconnectionTimeout));
    timer->async_wait(boost::bind(&CircuitBreakerConnection::OnTimerAlarm, this, clientName, timerIndex));
}

void CircuitBreakerConnection::OnTimerAlarm(const std::string &clientName, size_t timerIndex)
{
    LoggerFactory::GetLogger()->LogInfo((std::string("Try reconnect to client: ") + clientName).c_str());
    bool mayConnect = false;
    try
    {
        auto sock = ClientServerConnection::ConnetClientSocket(clientName);
        mayConnect = sock->is_open();
        sock->close();
    }
    catch (...)
    {
        mayConnect = false;
    }

    m_freeTimersIndexes.push_back(timerIndex);
    if (mayConnect)
    {
        LoggerFactory::GetLogger()->LogInfo((std::string("Reconnected to client: ") + clientName).c_str());
        m_clientName2FailedRequests[clientName] = 0;
    }
    else
    {
        LoggerFactory::GetLogger()->LogError((std::string("Failed reconnect to client: ") + clientName).c_str());
        SetTimer(clientName);
    }
}
