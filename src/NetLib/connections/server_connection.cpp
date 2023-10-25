#include "server_connection.h"

#include <config/base_sections.h>
#include <exceptions/server_exceptions.h>
#include <logger/LoggerFactory.h>

#include <boost/thread.hpp>
#include <chrono>
#include <iostream>

#include <network/IServerSession.h>
#include "std_future.hpp"

ServerConnection::ServerConnection(asio::io_context &context, const std::shared_ptr<IServerSessionCreator> &creator, const IConfigPtr &config)
    : m_sessionCreator(creator), m_acceptor(context), m_context(context)
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

void ServerConnection::ClearExpiredConnections()
{
    using namespace std::chrono_literals;

    for (size_t i = 0; i < m_coroutineSessions.size(); i++)
    {
        m_coroutineSessionsEraseMutex.lock();
        if (m_coroutineSessions[i].second.wait_for(0s) == std::future_status::ready)
        {
            m_coroutineSessions.erase(m_coroutineSessions.begin() + i);
            LoggerFactory::GetLogger()->LogInfo("erase session");
        }
        m_coroutineSessionsEraseMutex.unlock();
    }
}

void ServerConnection::Run()
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

void ServerConnection::AcceptNew()
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
            LoggerFactory::GetLogger()->LogInfo("start new session");
            auto session = m_sessionCreator->CreateSession();
            auto fut = session->Run(std::move(sock));
            m_coroutineSessions.push_back(coroutine_session_t(session, std::move(fut)));

            AcceptNew();
        });
}

void ServerConnection::Fail(const error_code &ec, const std::string &desc)
{
    std::ostringstream oss;
    oss << "error code: " << ec.message() << " description: " << desc;
    LoggerFactory::GetLogger()->LogError(oss.str().c_str());

    throw ServerException(oss.str());
}
