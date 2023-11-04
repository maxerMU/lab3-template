#pragma once

#include "IQueueHandler.h"

#include <config/IConfig.h>
#include <network/net_asio.h>

#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>

class RabbitMqQueueHandler
    : public IQueueHandler
{
public:
    RabbitMqQueueHandler(const IConfigPtr& config, asio::io_context& context, const std::string& queueName);
    ~RabbitMqQueueHandler() = default;
    
    virtual bool Publish(const std::string &message, const std::string& routingKey) override;

private:
    void Connect();

    IConfigPtr m_config;
    asio::io_context &m_context;
    std::string m_queueName;

    AMQP::LibBoostAsioHandler m_handler;
    std::shared_ptr<AMQP::TcpConnection> m_connection;
    std::shared_ptr<AMQP::TcpChannel> m_channel;
};

