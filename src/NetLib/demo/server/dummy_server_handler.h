#pragma once

#include <request_handlers/IServerReqHandler.h>
#include <reqresp/IResponseFactory.h>

class DummyServerHandler : public IServerReqHandler
{
public:
    DummyServerHandler(const IResponseFactoryPtr &respFactory);
    virtual void HandleRequest(const std::shared_ptr<IRequest> &req) override;
    virtual void MakeResponse(const std::shared_ptr<IResponse> &resp) override;

private:
    IResponseFactoryPtr m_respFactory;
    IResponsePtr m_resp;
};