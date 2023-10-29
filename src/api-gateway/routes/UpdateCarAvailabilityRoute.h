#pragma once

#include "context/ApiGatewayContext.h"
#include <router/IClientServerRoute.h>

#include "dto/CarAvailabilityDTO.h"

class UpdateCarAvailabilityRoute : public IClientServerRoute
{
public:
    virtual void Init(const IRequestHandlerContextPtr &context) override;
    virtual void SetRequestParameters(const std::vector<std::string> &params) override;
    virtual void ProcessRequest(const IRequestPtr &request, std::string &clientName) override;
    virtual bool Rollback(const IRequestPtr& request, std::string& clientName) override;
    virtual ResponceType ProcessResponse(const IResponsePtr &responseFromClient) override;
    virtual ResponceType ProcessRollbackResponse(const IResponsePtr& responseFromClient) override;

private:
    const std::string GET_CAR_BASE_TARGET = "/api/v1/cars";

    ApiGatewayContextPtr m_context;

    CarAvailabilityDTO m_postedCar;
    bool m_isPostSuccessfull = false;
};