#pragma once

#include <config/IConfig.h>
#include <network/IServerSession.h>

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <vector>

#include <network/IConnection.h>

using coroutine_session_t = std::pair<std::shared_ptr<IServerSession>, std::future<void>>;

class ServerConnection : public IConnection, public std::enable_shared_from_this<ServerConnection>
{
public:
    ServerConnection(asio::io_context &context, const std::shared_ptr<IServerSessionCreator> &creator,
                     const IConfigPtr &config);

    virtual void Run() override;

    virtual ~ServerConnection() = default;

protected:
    void Fail(const error_code &ec, const std::string &desc);

private:
    std::vector<coroutine_session_t> m_coroutineSessions;
    std::mutex m_coroutineSessionsEraseMutex;
    std::shared_ptr<IServerSessionCreator> m_sessionCreator;
    tcp::acceptor m_acceptor;
    asio::io_context &m_context;
    std::shared_ptr<IConfig> m_config;

    void AcceptNew();
    void ClearExpiredConnections();
};
