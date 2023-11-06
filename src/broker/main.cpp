#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>

#include <memory>
#include <logger/LoggerFactory.h>
#include <exceptions/server_exceptions.h>

// #include <boost/asio/io_context.hpp>
#include <boost/asio/io_service.hpp>

void test(std::shared_ptr<AMQP::TcpChannel> m_channel, std::string queueName, std::string message)
{

    m_channel->bindQueue(queueName, queueName, "route").onSuccess([&](){
        std::cout << "binded" << std::endl;
    });

    m_channel->publish(queueName, "route", message);
}

int main()
{
    boost::asio::io_context context(2);

    AMQP::LibBoostAsioHandler m_handler(context);
    
    // TODO
    AMQP::Address address("amqp://guest:guest@localhost");
    auto m_connection = std::make_shared<AMQP::TcpConnection>(&m_handler, address);

    if (!m_connection->closed())
        LoggerFactory::GetLogger()->LogInfo("Connected to rabbit mq broker");
    else
        throw BrokerConnectionException("can't connect to rabbit mq");

    std::shared_ptr<AMQP::TcpChannel> m_channel = std::make_shared<AMQP::TcpChannel>(m_connection.get());

    std::string queueName = "test-1-queue";
    std::string message = "Hello";

    m_channel->declareQueue(queueName).onSuccess([&]()
    {
        LoggerFactory::GetLogger()->LogInfo((queueName + std::string(": queue declared")).c_str());
    }).onError([&](const char *msg)
    {
        std::ostringstream oss;
        oss << queueName << ": failed declare queue: " << msg;
        LoggerFactory::GetLogger()->LogError(oss.str().c_str());
    });

    m_channel->declareExchange(queueName, AMQP::fanout).onSuccess([&]()
    {
        LoggerFactory::GetLogger()->LogInfo((queueName + std::string(": queue declared")).c_str());
    }).onError([&](const char *msg)
    {
        std::ostringstream oss;
        oss << queueName << ": failed declare queue: " << msg;
        LoggerFactory::GetLogger()->LogError(oss.str().c_str());
    });

    auto timer = std::make_shared<boost::asio::deadline_timer>(context);
    timer->expires_from_now(boost::posix_time::seconds(2));
    timer->async_wait(boost::bind(test, m_channel, queueName, message));

    return context.run();
    // access to the boost asio handler
    // note: we suggest use of 2 threads - normally one is fin (we are simply demonstrating thread safety).
    // boost::asio::io_service service(4);

    // handler for libev
    // AMQP::LibBoostAsioHandler handler(service);
    
    // make a connection
    // AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://guest:guest@localhost/"));
    
    // // we need a channel too
    // AMQP::TcpChannel channel(&connection);
    
    // // create a temporary queue
    // channel.declareQueue(AMQP::exclusive).onSuccess([&connection](const std::string &name, uint32_t, uint32_t) {
    //     
    //     // report the name of the temporary queue
    //     std::cout << "declared queue " << name << std::endl;
    //     
    //     // now we can close the connection
    //     connection.close();
    // });
    
    // run the handler
    // a t the moment, one will need SIGINT to stop.  In time, should add signal handling through boost API.
    // return service.run();
}
