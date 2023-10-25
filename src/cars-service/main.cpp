#include <config/config_creators.h>
#include <logger/LoggerFactory.h>
#include <network/net.h>

#include <memory>

#include "bl/CarsFacade.h"
#include "da/CarsDA.h"
#include "gateway/CarsGateway.h"
#include "gateway/CarsServerHandler.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    std::shared_ptr<IConfig> config = CreateYAMLConfig(std::string(argv[1]));
    LoggerFactory::InitLogger(config);

    CarsFacade::Instance()->Init(CreateCarsRepository(config, CarsDBSection));

    SetupRouter();

    auto respFactory = CreateResponceFactory();
    auto handlerCreator = std::make_shared<ServerReqHandlerCreator<CarsServerHandler>>(respFactory);
    auto sessionCreator = CreateServerSessionCreator(handlerCreator);

    asio::io_context ioc;
    auto connection = CreateServerConnection(ioc, sessionCreator, config);
    connection->Run();

    return 0;
}
