#include "RentsFacade.h"

#include <exceptions/database_exceptions.h>
#include <exceptions/logic_exceptions.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#define FINISHED_RENT_STATUS "FINISHED"
#define CANCELED_RENT_STATUS "CANCELED"

std::shared_ptr<RentsFacade> RentsFacade::Instance()
{
    static std::shared_ptr<RentsFacade> instance(new RentsFacade());
    return instance;
}

void RentsFacade::Init(const IRentsRepositoryPtr &repository)
{
    m_repository = repository;
}

RentDTO RentsFacade::GetRent(const std::string &username, const std::string &rentalUid) const
{
    if (!m_repository)
        throw NotInitializedException("repository doesn't initilized");

    try
    {
        return m_repository->GetRent(username, rentalUid);
    }
    catch(const DatabaseNotFoundException &e)
    {
        throw(RentNotFoundException(e.what()));
    }
}

RentDTO RentsFacade::AddRent(const PostRentDTO &postRent, const std::string &username)
{
    if (!m_repository)
        throw NotInitializedException("repository doesn't initilized");

    boost::uuids::random_generator gen;
    boost::uuids::uuid uuid = gen();
    std::string uuidStr = boost::uuids::to_string(uuid);

    RentDTO rent{
        uuidStr,
        username,
        postRent.paymentUid,
        postRent.carUid,
        postRent.dateFrom,
        postRent.dateTo,
        "IN_PROGRESS"
    };

    m_repository->AddRent(rent);

    return rent;
}

void RentsFacade::FinishRent(const std::string &username, const std::string &rentalUid)
{
    UpdateRent(username, rentalUid, FINISHED_RENT_STATUS);
}

void RentsFacade::CancelRent(const std::string &username, const std::string &rentalUid)
{
    UpdateRent(username, rentalUid, CANCELED_RENT_STATUS);
}

void RentsFacade::UpdateRent(const std::string &username, const std::string &rentalUid, const std::string &status)
{
    if (!m_repository)
        throw NotInitializedException("repository doesn't initilized");
    
    try
    {
        m_repository->UpdateRentStatus(username, rentalUid, status);
    }
    catch(const DatabaseNotFoundException &e)
    {
        throw(RentNotFoundException(e.what()));
    }
}

RentsDTO RentsFacade::GetRents(const std::string& username) const
{
    if (!m_repository)
        throw NotInitializedException("repository doesn't initilized");

    return m_repository->GetRents(username);
}