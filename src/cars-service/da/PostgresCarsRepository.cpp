#include "PostgresCarsRepository.h"

#include <config/base_sections.h>
#include <exceptions/database_exceptions.h>
#include <logger/LoggerFactory.h>

PostgresCarsRepository::PostgresCarsRepository(const IConfigPtr &conf, const std::string &connectionSection)
{
    ReadConfig(conf, connectionSection);
    Connect();
    AddPrepareStatements();
}

CarsDTO PostgresCarsRepository::GetAllCars()
{
    pqxx::result rows;
    try
    {
        pqxx::work w(*m_connection);
        rows = w.exec_prepared(m_requestsNames[READ_ALL]);
        w.commit();
    }
    catch (std::exception &ex)
    {
        throw DatabaseExecutionException(ex.what());
    }

    CarsDTO cars;
    cars.reserve(rows.size());
    for (const auto &row : rows)
    {
        CarDTO car{
            row["id"].as<size_t>(),
            row["car_uid"].as<std::string>(),
            row["brand"].as<std::string>(),
            row["model"].as<std::string>(),
            row["registration_number"].as<std::string>(),
            row["power"].as<size_t>(),
            row["price"].as<size_t>(),
            row["type"].as<std::string>(),
            row["availability"].as<bool>()
        };

        cars.push_back(car);
    }

    return cars;
}

CarsDTO PostgresCarsRepository::GetAvailableCars()
{
    pqxx::result rows;
    try
    {
        pqxx::work w(*m_connection);
        rows = w.exec_prepared(m_requestsNames[READ_AVAILABLE]);
        w.commit();
    }
    catch (std::exception &ex)
    {
        throw DatabaseExecutionException(ex.what());
    }

    CarsDTO cars;
    cars.reserve(rows.size());
    for (const auto &row : rows)
    {
        CarDTO car{
            row["id"].as<size_t>(),
            row["car_uid"].as<std::string>(),
            row["brand"].as<std::string>(),
            row["model"].as<std::string>(),
            row["registration_number"].as<std::string>(),
            row["power"].as<size_t>(),
            row["price"].as<size_t>(),
            row["type"].as<std::string>(),
            row["availability"].as<bool>()
        };

        cars.push_back(car);
    }

    return cars;
}

CarDTO PostgresCarsRepository::GetCar(const std::string& carUid)
{
    pqxx::result rows;
    try
    {
        pqxx::work w(*m_connection);
        rows = w.exec_prepared(m_requestsNames[READ_BY_UID], carUid);
        w.commit();
    }
    catch (std::exception &ex)
    {
        throw DatabaseExecutionException(ex.what());
    }

    if (rows.empty())
        throw DatabaseNotFoundException(carUid.c_str());

    CarDTO car{
        rows[0]["id"].as<size_t>(),
        rows[0]["car_uid"].as<std::string>(),
        rows[0]["brand"].as<std::string>(),
        rows[0]["model"].as<std::string>(),
        rows[0]["registration_number"].as<std::string>(),
        rows[0]["power"].as<size_t>(),
        rows[0]["price"].as<size_t>(),
        rows[0]["type"].as<std::string>(),
        rows[0]["availability"].as<bool>()
    };

    return car;
}

void PostgresCarsRepository::UpdateCarAvailability(const std::string &carUid, bool available)
{
    try
    {
        pqxx::work w(*m_connection);
        w.exec_prepared(m_requestsNames[UPDATE_CAR_AVALABILITY], carUid, available);
        w.commit();
    }
    catch (std::exception &ex)
    {
        throw DatabaseExecutionException(ex.what());
    }
}

void PostgresCarsRepository::ReadConfig(const IConfigPtr &conf, const std::string &connectionSection)
{
    m_name = conf->GetStringField({connectionSection, DbNameSection});
    m_user = conf->GetStringField({connectionSection, DbUserSection});
    m_userPassword = conf->GetStringField({connectionSection, DbUserPasswordSection});
    m_host = conf->GetStringField({connectionSection, DbHostSection});
    m_port = conf->GetUintField({connectionSection, DbPortSection});
}

void PostgresCarsRepository::Connect()
{
    std::string connectionString = "dbname = " + m_name + " user = " + m_user + " password = " + m_userPassword +
                                   " host = " + m_host + " port = " + std::to_string(m_port);

    try
    {
        m_connection = std::shared_ptr<pqxx::connection>(new pqxx::connection(connectionString.c_str()));

        if (!m_connection->is_open())
            throw DatabaseConnectException("can't connect to " + m_name);
        else
            LoggerFactory::GetLogger()->LogInfo((std::string("Connected to db ") + m_name).c_str());
    }
    catch (std::exception &ex)
    {
        throw DatabaseConnectException("can't connect to " + m_name + " " + ex.what());
    }
}

void PostgresCarsRepository::AddPrepareStatements()
{
    m_connection->prepare(m_requestsNames[READ_ALL], "SELECT * FROM cars");
    m_connection->prepare(m_requestsNames[READ_AVAILABLE], "SELECT * FROM cars WHERE availability=true");
    m_connection->prepare(m_requestsNames[READ_BY_UID], "SELECT * FROM cars WHERE car_uid=$1");
    m_connection->prepare(m_requestsNames[UPDATE_CAR_AVALABILITY], "UPDATE cars SET availability=$2 WHERE car_uid=$1");
}