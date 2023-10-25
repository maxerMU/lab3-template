#include "PaymentsFacade.h"

#include <exceptions/database_exceptions.h>
#include <exceptions/logic_exceptions.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#define PAID_PAYMENT_STATUS "PAID"
#define CANCELED_PAYMENT_STATUS "CANCELED"

std::shared_ptr<PaymentsFacade> PaymentsFacade::Instance()
{
    static std::shared_ptr<PaymentsFacade> instance(new PaymentsFacade());
    return instance;
}

void PaymentsFacade::Init(const IPaymentsRepositoryPtr &repository)
{
    m_repository = repository;
}

PaymentDTO PaymentsFacade::GetPayment(const std::string &uid) const
{
    if (!m_repository)
        throw NotInitializedException("repository doesn't initilized");
    
    try
    {
        return m_repository->GetPayment(uid);
    }
    catch(const DatabaseNotFoundException& e)
    {
        throw PaymentNotFoundException(e.what());
    }
}

PaymentDTO PaymentsFacade::AddPayment(const PostPaymentDTO &payment) const
{
    if (!m_repository)
        throw NotInitializedException("repository doesn't initilized");
    
    boost::uuids::random_generator gen;
    boost::uuids::uuid uuid = gen();

    std::string uuidStr = boost::uuids::to_string(uuid);
    PaymentDTO paymentResult{uuidStr, PAID_PAYMENT_STATUS, payment.price};
    m_repository->AddPayment(paymentResult);

    return paymentResult;
}

void PaymentsFacade::CancelPayment(const std::string &uid) const
{
    if (!m_repository)
        throw NotInitializedException("repository doesn't initilized");
    
    m_repository->UpdatePayment(uid, CANCELED_PAYMENT_STATUS);
}
