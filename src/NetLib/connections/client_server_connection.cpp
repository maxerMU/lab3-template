#include "client_server_connection.h"

#include <config/base_sections.h>
#include <exceptions/server_exceptions.h>
#include <logger/LoggerFactory.h>

#include <boost/thread.hpp>
#include <chrono>
#include <iostream>

ClientServerConnection::ClientServerConnection(asio::io_context &context,
                                               const std::shared_ptr<IClientServerSessionCreator> &creator,
                                               const std::shared_ptr<IConfig> &config)
    : m_sessionCreator(creator), m_acceptor(context), m_context(context), m_config(config)
{
    m_clientSockets = ConnetClientSockets(m_config);
    ConnectServerSocket(config);
}

std::vector<std::shared_ptr<tcp::socket>> ClientServerConnection::ConnetClientSockets(
    const std::shared_ptr<IConfig> &config)
{
    std::vector<std::shared_ptr<tcp::socket>> client_sockets;
    std::vector<std::string> clients = config->GetStringArray({ClientsSection});

    for (auto client : clients)
    {
        std::ostringstream oss;
        oss << "Connecting to " << client << "...";
        LoggerFactory::GetLogger()->LogInfo(oss.str().c_str());

        std::string host = config->GetStringField({client, ClientHostSection});
        int port = config->GetIntField({client, ClientPortSection});
        asio::ip::tcp::resolver resolver(m_context);
        auto results = resolver.resolve(host, std::to_string(port));
        // tcp::endpoint ep(asio::ip::address::from_string(host), port);

        std::shared_ptr<tcp::socket> client_sock(new tcp::socket(m_context));
        asio::connect(*client_sock, results.begin(), results.end());

        client_sockets.push_back(client_sock);
        LoggerFactory::GetLogger()->LogInfo("Connected");
    }

    return client_sockets;
}

void ClientServerConnection::ConnectServerSocket(const std::shared_ptr<IConfig> &config)
{
    error_code ec;

    int port = config->GetIntField({ServerSection, PortSection});

    tcp::endpoint ep(asio::ip::address_v4(), port);

    m_acceptor.open(ep.protocol(), ec);
    if (ec)
    {
        Fail(ec, "open acceptor");
        return;
    }

    m_acceptor.set_option(asio::socket_base::reuse_address(true), ec);
    if (ec)
    {
        Fail(ec, "set option");
        return;
    }

    m_acceptor.bind(ep, ec);
    if (ec)
    {
        Fail(ec, "bind");
        return;
    }

    m_acceptor.listen(asio::socket_base::max_listen_connections, ec);
    if (ec)
    {
        Fail(ec, "listen");
        return;
    }
}

void ClientServerConnection::ClearExpiredConnections()
{
    using namespace std::chrono_literals;

    for (size_t i = 0; i < m_coroutineSessions.size(); i++)
    {
        m_coroutineSessionsEraseMutex.lock();
        if (m_coroutineSessions[i].second.wait_for(0s) == std::future_status::ready)
        {
            LoggerFactory::GetLogger()->LogInfo("erase session");
            m_coroutineSessions.erase(m_coroutineSessions.begin() + i);
        }
        m_coroutineSessionsEraseMutex.unlock();
    }
}

void ClientServerConnection::Run()
{
    AcceptNew();
    m_context.run();
    // boost::thread_group tg;
    // accept_new();
    // for (size_t i = 0; i < 12; i++) {
    //   tg.create_thread([&]() { context_.run(); });
    // }
    // // context_.run();
    // tg.join_all();
}

void ClientServerConnection::AcceptNew()
{
    ClearExpiredConnections();

    m_acceptor.async_accept(
        [this](error_code ec, tcp::socket sock)
        {
            if (ec)
            {
                Fail(ec, "accept");
                return;
            }
            auto remoteEndpoint = sock.remote_endpoint();
            remoteEndpoint.address().to_string();
            std::ostringstream oss;
            oss << "start new session: " << remoteEndpoint.address().to_string();
            LoggerFactory::GetLogger()->LogInfo(oss.str().c_str());
            auto session = m_sessionCreator->CreateSession();
            auto fut = session->Run(std::move(sock), m_clientSockets);
            m_coroutineSessions.push_back(coroutine_cssession_t(session, std::move(fut)));

            AcceptNew();
        });
}

void ClientServerConnection::Fail(const error_code &ec, const std::string &desc)
{
    std::ostringstream oss;
    oss << "error code: " << ec.message() << " description: " << desc;
    LoggerFactory::GetLogger()->LogError(oss.str().c_str());

    throw ServerException(oss.str());
}
