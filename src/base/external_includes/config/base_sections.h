#pragma once

#include <string>

const std::string LogFileSection = "LogFile";

// ===================

const std::string ServerSection = "Server";
const std::string PortSection = "Port";
const std::string ClientsSection = "Clients";
const std::string ClientHostSection = "Host";
const std::string ClientPortSection = "Port";

// ===================

const std::string DbHostSection = "DB_HOST";
const std::string DbNameSection = "DB_NAME";
const std::string DbPortSection = "DB_PORT";
const std::string DbUserSection = "DB_USER";
const std::string DbUserPasswordSection = "DB_USER_PASSWORD";

// ===================

const std::string CIRCUIT_BREAKER_SECTION = "CIRCUIT_BREAKER";
const std::string CIRCUIT_BREAKER_RETRY_COUNT_SECTION = "RETRY_COUNT";
const std::string CIRCUIT_BREAKER_RECONNECT_TIMEOUT_SECTION = "RECONNECT_TIMEOUT";

// ===================

const std::string BROKER_SECTION = "Broker";
const std::string BROKER_USER_SECTION = "User";
const std::string BROKER_USER_PASSWORD_SECTION = "Password";
const std::string BROKER_HOST_SECTION = "Host";

const std::string BROKER_CANCEL_RENT_QUEUE_SECTION = "CancelRentQueue";

// ===================

const std::string UsedCriterions = "UsedCriterions";
const std::string AuthSection = "Auth";
const std::string JWTTokenLifeTime = "JWTTokenLifeTime";
const std::string JWTAlgorithm = "JWTAlgorithm";
const std::string JWTSecret = "JWTSecret";
