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
    : m_context(context), m_sessionCreator(creator), m_acceptor(context), m_config(config)
{
    ConnectServerSocket(config);
}

std::shared_ptr<tcp::socket> ClientServerConnection::ConnetClientSocket(const std::string &clientName)
{
    std::shared_ptr<tcp::socket> clientSocket;
    std::vector<std::string> clients = m_config->GetStringArray({ClientsSection});

    auto clientIt = std::find(clients.begin(), clients.end(), clientName);
    if (clientIt == clients.end())
        throw ClientNotFoundException(clientName);

    std::ostringstream oss;
    oss << "Connecting to " << clientName << "...";
    LoggerFactory::GetLogger()->LogInfo(oss.str().c_str());

    std::string host = m_config->GetStringField({clientName, ClientHostSection});
    int port = m_config->GetIntField({clientName, ClientPortSection});
    asio::ip::tcp::resolver resolver(m_context);
    auto results = resolver.resolve(host, std::to_string(port));

    std::shared_ptr<tcp::socket> clientSock(new tcp::socket(m_context));
    try
    {
        asio::connect(*clientSock, results.begin(), results.end());
    }
    catch (...)
    {
        oss.clear();
        oss << "Can't connect to " << clientName << ": " << host << ":" << port;
        LoggerFactory::GetLogger()->LogError(oss.str().c_str());

        throw ClientNotConnectedException(oss.str().c_str());
    }

    oss.clear();
    oss << "Connected to " << clientName;
    LoggerFactory::GetLogger()->LogInfo(oss.str().c_str());

    return clientSock;
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
            auto fut = session->Run(std::move(sock), shared_from_this());
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
