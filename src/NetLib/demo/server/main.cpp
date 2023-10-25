#include <network/net.h>
#include <config/config_creators.h>
#include <logger/LoggerFactory.h>
#include <router/router.h>

#include <memory>

#include "dummy_server_handler.h"

void Check(const IResponsePtr &resp, const IRequestPtr &, const std::vector<std::string> &params)
{
    if (params.size() != 1)
        return resp->SetStatus(net::CODE_503);

    resp->SetBody(params[0]);
    resp->SetStatus(net::CODE_200);
}

void Info(const IResponsePtr &resp, const IRequestPtr &)
{
    resp->SetBody("status: ok");
    resp->SetStatus(net::CODE_200);
}

void SetupRouter()
{
    RequestsRouter::Instanse()->AddStaticEndpoint({"/info", net::GET}, Info);
    RequestsRouter::Instanse()->AddDynamicEndpoint({std::regex("/check/([0-9\\-]+)"), net::GET}, Check);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    SetupRouter();

    std::shared_ptr<IConfig> config = CreateYAMLConfig(std::string(argv[1]));
    LoggerFactory::InitLogger(config);

    auto respFactory = CreateResponceFactory();
    auto handlerCreator = std::make_shared<ServerReqHandlerCreator<DummyServerHandler>>(respFactory);
    auto sessionCreator = CreateServerSessionCreator(handlerCreator);

    asio::io_context ioc;
    auto connection = CreateServerConnection(ioc, sessionCreator, config);
    connection->Run();

    return 0;
}
