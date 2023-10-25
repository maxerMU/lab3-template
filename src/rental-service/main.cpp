#include <config/config_creators.h>
#include <logger/LoggerFactory.h>
#include <network/net.h>

#include <memory>

#include "bl/RentsFacade.h"
#include "da/RentsDA.h"
#include "gateway/RentsGateway.h"
#include "gateway/RentsServerHandler.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    std::shared_ptr<IConfig> config = CreateYAMLConfig(std::string(argv[1]));
    LoggerFactory::InitLogger(config);

    RentsFacade::Instance()->Init(CreateRentsRepository(config, RentsDBSection));

    SetupRouter();

    auto respFactory = CreateResponceFactory();
    auto handlerCreator = std::make_shared<ServerReqHandlerCreator<RentsServerHandler>>(respFactory);
    auto sessionCreator = CreateServerSessionCreator(handlerCreator);

    asio::io_context ioc;
    auto connection = CreateServerConnection(ioc, sessionCreator, config);
    connection->Run();

    return 0;
}
