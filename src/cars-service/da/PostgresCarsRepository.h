#pragma once

#include <map>
#include <pqxx/pqxx>

#include "ICarsRepository.h"
#include <config/IConfig.h>

class PostgresCarsRepository : public ICarsRepository
{
public:
    PostgresCarsRepository(const IConfigPtr &conf, const std::string &connectionSection);

    virtual CarsDTO GetAllCars() override;
    virtual CarsDTO GetAvailableCars() override;
    virtual CarDTO GetCar(const std::string& carUid) override;
    virtual void UpdateCarAvailability(const std::string& carUid, bool available) override;

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
        READ_AVAILABLE,
        READ_ALL,
        READ_BY_UID,
        UPDATE_CAR_AVALABILITY
    };

    std::map<PreparedRequests, std::string> m_requestsNames = {
        {READ_AVAILABLE, "get_available_cars"},
        {READ_ALL, "get_all_cars"},
        {READ_BY_UID, "get_by_uid"},
        {UPDATE_CAR_AVALABILITY, "update_avalability"}
    };
};