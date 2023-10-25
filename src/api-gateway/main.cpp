#include <config/config_creators.h>
#include <logger/LoggerFactory.h>
#include <network/net.h>
#include <router/router.h>

#include <memory>

#include "ApiGatewayHandler.h"
#include "routes/GetCarsRoute.h"
#include "routes/GetCarRoute.h"
#include "routes/GetPaymentRoute.h"
#include "routes/GetRentsRoute.h"
#include "routes/GetRentPrep.h"
#include "routes/GetRentsPrep.h"
#include "routes/GetRentPost.h"
#include "routes/GetRentsPost.h"
#include "routes/GetRentRoute.h"
#include "routes/PostRentPrep.h"
#include "routes/PostRentRoute.h"
#include "routes/PostRentPost.h"
#include "routes/PostPaymentRoute.h"
#include "routes/UpdateCarAvailabilityRoute.h"
#include "routes/FinishRentPrep.h"
#include "routes/FinishRentRoute.h"
#include "routes/CancelRentPrep.h"
#include "routes/CancelRentRoute.h"
#include "routes/CancelPaymentRoute.h"
#include "routes/HealthRoute.h"

void SetupRouter()
{
    std::vector<IClientServerRouteCreatorPtr> healthRoute{
        std::make_shared<ClientServerRouteCreator<HealthRoute>>(),
    };
    RequestsRouter::Instanse()->AddStaticEndpoint({"/manage/health", net::GET}, healthRoute);

    std::vector<IClientServerRouteCreatorPtr> getCarsRoute{
        std::make_shared<ClientServerRouteCreator<GetCarsRoute>>()
    };
    std::regex getCarsTarget("/api/v1/cars\\?page=([0-9\\-]+)&size=([0-9\\-]+)&showAll=(true|false)");
    RequestsRouter::Instanse()->AddDynamicEndpoint({getCarsTarget, net::GET}, getCarsRoute);

    std::vector<IClientServerRouteCreatorPtr> getRentsRoute{
        std::make_shared<ClientServerRouteCreator<GetRentsPrep>>(),
        std::make_shared<ClientServerRouteCreator<GetRentsRoute>>(),
        std::make_shared<ClientServerRouteCreator<GetCarRoute>>(),
        std::make_shared<ClientServerRouteCreator<GetPaymentRoute>>(),
        std::make_shared<ClientServerRouteCreator<GetRentsPost>>()
    };
    RequestsRouter::Instanse()->AddStaticEndpoint({"/api/v1/rental", net::GET}, getRentsRoute);

    std::vector<IClientServerRouteCreatorPtr> getRentRoute{
        std::make_shared<ClientServerRouteCreator<GetRentPrep>>(),
        std::make_shared<ClientServerRouteCreator<GetRentRoute>>(),
        std::make_shared<ClientServerRouteCreator<GetCarRoute>>(),
        std::make_shared<ClientServerRouteCreator<GetPaymentRoute>>(),
        std::make_shared<ClientServerRouteCreator<GetRentPost>>()
    };
    RequestsRouter::Instanse()->AddDynamicEndpoint({std::regex("/api/v1/rental/([0-9\\-a-z]+)"), net::GET}, getRentRoute);

    std::vector<IClientServerRouteCreatorPtr> postRentRoute{
        std::make_shared<ClientServerRouteCreator<PostRentPrep>>(),
        std::make_shared<ClientServerRouteCreator<GetCarRoute>>(),
        std::make_shared<ClientServerRouteCreator<UpdateCarAvailabilityRoute>>(),
        std::make_shared<ClientServerRouteCreator<PostPaymentRoute>>(),
        std::make_shared<ClientServerRouteCreator<PostRentRoute>>(),
        std::make_shared<ClientServerRouteCreator<PostRentPost>>()
    };
    RequestsRouter::Instanse()->AddStaticEndpoint({"/api/v1/rental", net::POST}, postRentRoute);

    std::vector<IClientServerRouteCreatorPtr> finishRentRoute{
        std::make_shared<ClientServerRouteCreator<FinishRentPrep>>(),
        std::make_shared<ClientServerRouteCreator<GetRentRoute>>(),
        std::make_shared<ClientServerRouteCreator<UpdateCarAvailabilityRoute>>(),
        std::make_shared<ClientServerRouteCreator<FinishRentRoute>>()
    };
    RequestsRouter::Instanse()->AddDynamicEndpoint({std::regex("/api/v1/rental/([0-9\\-a-z]+)/finish"), net::POST}, finishRentRoute);

    std::vector<IClientServerRouteCreatorPtr> cancelRentRoute{
        std::make_shared<ClientServerRouteCreator<CancelRentPrep>>(),
        std::make_shared<ClientServerRouteCreator<GetRentRoute>>(),
        std::make_shared<ClientServerRouteCreator<UpdateCarAvailabilityRoute>>(),
        std::make_shared<ClientServerRouteCreator<CancelPaymentRoute>>(),
        std::make_shared<ClientServerRouteCreator<CancelRentRoute>>()
    };
    RequestsRouter::Instanse()->AddDynamicEndpoint({std::regex("/api/v1/rental/([0-9\\-a-z]+)"), net::DELETE}, cancelRentRoute);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    SetupRouter();

    asio::io_context ioc;

    std::shared_ptr<IConfig> config = CreateYAMLConfig(std::string(argv[1]));
    LoggerFactory::InitLogger(config);

    auto handlerCreator = std::make_shared<ClientServerReqHandlerCreator<ApiGatewayHandler>>(config);
    auto sessionCreator = CreateClientServerSessionCreator(handlerCreator);
    auto connection = CreateClientServerConnection(ioc, sessionCreator, config);

    connection->Run();

    return 0;
}
