#pragma once

#include "IPaymentsRepository.h"
#include <config/IConfig.h>

const std::string PaymentsDBSection = "DB_Payments";

IPaymentsRepositoryPtr CreatePaymentsRepository(const IConfigPtr &conf, const std::string &connectionSection);