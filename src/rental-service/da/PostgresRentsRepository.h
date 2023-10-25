#pragma once

#include <map>
#include <pqxx/pqxx>

#include "IRentsRepository.h"
#include <config/IConfig.h>

class PostgresRentsRepository : public IRentsRepository
{
public:
    PostgresRentsRepository(const IConfigPtr &conf, const std::string &connectionSection);

    virtual RentsDTO GetRents(const std::string& username) override;
    virtual RentDTO GetRent(const std::string& username, const std::string& rentalUid) override;
    virtual void AddRent(const RentDTO& rent) override;
    virtual void UpdateRentStatus(const std::string& username, const std::string& rentalUid, const std::string& status) override;

private:
    void ReadConfig(const IConfigPtr &conf, const std::string &connectionSection);

    void Connect();
    void AddPrepareStatements();

    std::shared_ptr<pqxx::connection> m_connection;

    std::string m_name;
    std::string m_user;
    std::string m_userPassword;
    std::string m_host;
    size_t m_port;

    enum PreparedRequests
    {
        READ,
        READ_ALL,
        WRITE,
        UPDATE
    };

    std::map<PreparedRequests, std::string> m_requestsNames = {
        {READ, "get_rent"},
        {READ_ALL, "get_all_rents"},
        {WRITE, "add_rent"},
        {UPDATE, "update_rent"},
    };
};