#ifndef CLIENTSERVERCONNECTION_H
#define CLIENTSERVERCONNECTION_H

#include <network/IConnection.h>
#include <config/IConfig.h>

#include <network/IClientServerSession.h>
#include <network/net_asio.h>

using coroutine_cssession_t = std::pair<std::shared_ptr<IClientServerSession>, std::future<void>>;

class ClientServerConnection
    : public IConnection
    , public std::enable_shared_from_this<ClientServerConnection>
{
public:
    ClientServerConnection(asio::io_context &context, const std::shared_ptr<IClientServerSessionCreator> &creator,
                           const std::shared_ptr<IConfig> &config);

    void Run();

protected:
    void Fail(const error_code &ec, const std::string &desc);

private:
    void ConnectServerSocket(const std::shared_ptr<IConfig> &config);
    std::vector<std::shared_ptr<tcp::socket>> ConnetClientSockets(const std::shared_ptr<IConfig> &config);
    void AcceptNew();
    void ClearExpiredConnections();

    std::vector<coroutine_cssession_t> m_coroutineSessions;
    std::mutex m_coroutineSessionsEraseMutex;
    std::vector<std::shared_ptr<tcp::socket>> m_clientSockets;
    std::shared_ptr<IClientServerSessionCreator> m_sessionCreator;
    tcp::acceptor m_acceptor;
    asio::io_context &m_context;
    std::shared_ptr<IConfig> m_config;
};

#endif // CLIENTSERVERCONNECTION_H
