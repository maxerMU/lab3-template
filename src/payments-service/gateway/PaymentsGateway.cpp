#include "PaymentsGateway.h"

#include "bl/PaymentsFacade.h"
#include <exceptions/logic_exceptions.h>
#include <logger/LoggerFactory.h>
#include <router/router.h>

void Health(const IResponsePtr &resp, const IRequestPtr &)
{
    resp->SetStatus(net::CODE_200);
}

void GetPayment(const IResponsePtr &resp, const IRequestPtr &, const std::vector<std::string>& params)
{
    if (params.size() != 1)
    {
        LoggerFactory::GetLogger()->LogError("get payment incorrect params count");
        resp->SetStatus(net::CODE_400);
        return;
    }

    try
    {
        PaymentDTO payment = PaymentsFacade::Instance()->GetPayment(params[0]);
        resp->SetStatus(net::CODE_200);
        resp->SetBody(payment.ToJSON());
    }
    catch(const PaymentNotFoundException& e)
    {
        LoggerFactory::GetLogger()->LogWarning((std::string("payment not found: ") + e.what()).c_str());
        resp->SetStatus(net::CODE_404);
    }
}

void AddPayment(const IResponsePtr &resp, const IRequestPtr &request)
{
    PostPaymentDTO postPaymentDTO;
    postPaymentDTO.FromJSON(request->GetBody());

    PaymentDTO payment = PaymentsFacade::Instance()->AddPayment(postPaymentDTO);
    resp->SetStatus(net::CODE_200);
    resp->SetBody(payment.ToJSON());
}

void CancelPayment(const IResponsePtr &resp, const IRequestPtr &, const std::vector<std::string>& params)
{
    if (params.size() != 1)
    {
        LoggerFactory::GetLogger()->LogError("get payment incorrect params count");
        resp->SetStatus(net::CODE_400);
        return;
    }

    PaymentsFacade::Instance()->CancelPayment(params[0]);
    resp->SetStatus(net::CODE_200);
}

void SetupRouter()
{
    RequestsRouter::Instanse()->AddStaticEndpoint({"/manage/health", net::GET}, Health);
    RequestsRouter::Instanse()->AddDynamicEndpoint({std::regex("/api/v1/payments/([0-9\\-a-z]+)"), net::GET}, GetPayment);
    RequestsRouter::Instanse()->AddStaticEndpoint({"/api/v1/payments", net::POST}, AddPayment);
    RequestsRouter::Instanse()->AddDynamicEndpoint({std::regex("/api/v1/payments/([0-9\\-a-z]+)"), net::DELETE}, CancelPayment);
}