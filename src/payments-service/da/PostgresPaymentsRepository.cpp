#include "PostgresPaymentsRepository.h"

#include <config/base_sections.h>
#include <exceptions/database_exceptions.h>
#include <logger/LoggerFactory.h>

PostgresPaymentsRepository::PostgresPaymentsRepository(const IConfigPtr &conf, const std::string &connectionSection)
{
    ReadConfig(conf, connectionSection);
    Connect();
    AddPrepareStatements();
}

PaymentDTO PostgresPaymentsRepository::GetPayment(const std::string &uid)
{
    pqxx::result rows;
    try
    {
        pqxx::work w(*m_connection);
        rows = w.exec_prepared(m_requestsNames[READ], uid);
        w.commit();
    }
    catch (std::exception &ex)
    {
        throw DatabaseExecutionException(ex.what());
    }

    if (rows.size() != 1)
        throw DatabaseNotFoundException("payment not found");

    PaymentDTO payment{
        rows[0]["payment_uid"].as<std::string>(),
        rows[0]["status"].as<std::string>(),
        rows[0]["price"].as<size_t>(),
    };

    return payment;
}

void PostgresPaymentsRepository::AddPayment(const PaymentDTO &payment)
{
    try
    {
        pqxx::work w(*m_connection);
        w.exec_prepared(m_requestsNames[WRITE], payment.paymentUid, payment.status, payment.price);
        w.commit();
    }
    catch (std::exception &ex)
    {
        throw DatabaseExecutionException(ex.what());
    }
}

void PostgresPaymentsRepository::UpdatePayment(const std::string &paymentUid, const std::string &status)
{
    try
    {
        pqxx::work w(*m_connection);
        w.exec_prepared(m_requestsNames[UPDATE], paymentUid, status);
        w.commit();
    }
    catch (std::exception &ex)
    {
        throw DatabaseExecutionException(ex.what());
    }
}

void PostgresPaymentsRepository::ReadConfig(const IConfigPtr &conf, const std::string &connectionSection)
{
    m_name = conf->GetStringField({connectionSection, DbNameSection});
    m_user = conf->GetStringField({connectionSection, DbUserSection});
    m_userPassword = conf->GetStringField({connectionSection, DbUserPasswordSection});
    m_host = conf->GetStringField({connectionSection, DbHostSection});
    m_port = conf->GetUintField({connectionSection, DbPortSection});
}

void PostgresPaymentsRepository::Connect()
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

void PostgresPaymentsRepository::AddPrepareStatements()
{
    m_connection->prepare(m_requestsNames[READ], "SELECT * FROM payment WHERE payment_uid=$1");
    m_connection->prepare(m_requestsNames[WRITE], "INSERT INTO payment(payment_uid, status, price) VALUES ($1, $2, $3)");
    m_connection->prepare(m_requestsNames[UPDATE], "UPDATE payment SET status=$2 where payment_uid=$1");
}