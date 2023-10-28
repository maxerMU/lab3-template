#pragma once

#include <map>

#include "client_server_connection.h"

class CircuitBreakerConnection
    : public ClientServerConnection
{
public:
    CircuitBreakerConnection(asio::io_context &context, const IClientServerSessionCreatorPtr &creator, const IConfigPtr &config);

    virtual std::shared_ptr<tcp::socket> ConnetClientSocket(const std::string &clientName) override;

private:
    void ReadConfig(const IConfigPtr& config);
    void SetTimer(const std::string& clientName);
    void OnTimerAlarm(const std::string& clientName, size_t timerIndex);

    size_t m_maxRetryCount;
    size_t m_reconnectionTimeout;

    std::map<std::string, size_t> m_clientName2FailedRequests;

    std::vector<std::shared_ptr<asio::deadline_timer>> m_timersPool;
    std::vector<size_t> m_freeTimersIndexes;
};

