#pragma once

#include <memory>

#include <da/IRentsRepository.h>
#include <dto/RentDTO.h>
#include <dto/PostRentDTO.h>

class RentsFacade
{
public:
    static std::shared_ptr<RentsFacade> Instance();
    void Init(const IRentsRepositoryPtr &repository);

    RentsDTO GetRents(const std::string& username) const;
    RentDTO GetRent(const std::string& username, const std::string& rentalUid) const;
    RentDTO AddRent(const PostRentDTO& postRent, const std::string& username);
    void FinishRent(const std::string& username, const std::string& rentalUid);
    void CancelRent(const std::string& username, const std::string& rentalUid);

private:
    RentsFacade() = default;
    void UpdateRent(const std::string& username, const std::string& rentalUid, const std::string& status);

    IRentsRepositoryPtr m_repository;
};