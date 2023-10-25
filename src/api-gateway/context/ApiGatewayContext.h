#pragma once

#include <router/IRequestHandlerContext.h>

#include "dto/RentDTO.h"
#include "dto/CarDTO.h"
#include "dto/PaymentDTO.h"
#include "dto/PostRentCarDTO.h"

class ApiGatewayContext 
    : public IRequestHandlerContext
{
public:
    ApiGatewayContext();
    virtual ~ApiGatewayContext() = default;
    virtual IRequestPtr GetCurrentRequest() override;
    virtual IResponsePtr GetCurrentResponse() override;

    enum RequestType
    {
        Undefined,
        GetRent,
        GetRents,
        PostRent,
        FinishRent,
        CancelRent
    };
    RequestType GetRequestType() const;
    void SetRequestType(const RequestType& reqType);

    // TODO boost variant
    struct RequestProcessInfo
    {
        struct GetRentRequest
        {
            std::string rentUid;
            RentDTO rent;
            CarDTO car;
            PaymentDTO payment;
        } getRentRequest;

        struct GetRentsRequest
        {
            RentsDTO rents;
            CarsDTO cars;
            PaymentsDTO payments;
        } getRentsRequest;
        
        struct PostRentRequest
        {
            PostRentCarDTO postRent;
            CarDTO car;
            PaymentDTO payment;
            RentDTO rent;
        } postRentRequest;
        
        struct FinishRentRequest
        {
            std::string rentUid;
            RentDTO rent;
        } finishRentRequest;
        
        struct CancelRentRequest
        {
            std::string rentUid;
            RentDTO rent;
        } cancelRentRequest;
    };
    RequestProcessInfo& GetProcessInfo();
    
private:
    IRequestHandlerContextPtr m_context;

    RequestType m_requestType = Undefined;
    RequestProcessInfo m_requestProcessInfo;
};

using ApiGatewayContextPtr = std::shared_ptr<ApiGatewayContext>;