#pragma once

#include "ICarsRepository.h"
#include <config/IConfig.h>

const std::string CarsDBSection = "DB_CARS";

ICarsRepositoryPtr CreateCarsRepository(const IConfigPtr &conf, const std::string &connectionSection);