#pragma once

#include <memory>

#include <dto/CarDTO.h>

class ICarsRepository
{
public:
    virtual ~ICarsRepository() = default;

    virtual CarsDTO GetAllCars() = 0;
    virtual CarsDTO GetAvailableCars() = 0;
    virtual CarDTO GetCar(const std::string& carUid) = 0;
    virtual void UpdateCarAvailability(const std::string& carUid, bool available) = 0;
};

using ICarsRepositoryPtr = std::shared_ptr<ICarsRepository>;