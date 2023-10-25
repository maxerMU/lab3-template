#include "CarsFacade.h"

#include <exceptions/database_exceptions.h>
#include <exceptions/logic_exceptions.h>

std::shared_ptr<CarsFacade> CarsFacade::Instance()
{
    static std::shared_ptr<CarsFacade> instance(new CarsFacade());
    return instance;
}

void CarsFacade::Init(const ICarsRepositoryPtr &repository)
{
    m_repository = repository;
}

CarsDTO CarsFacade::GetCars(size_t page, size_t pageSize, bool showAll) const
{
    if (!m_repository)
        throw NotInitializedException("repository doesn't initilized");

    CarsDTO cars;
    if (showAll)
        cars = m_repository->GetAllCars();
    else
        cars = m_repository->GetAvailableCars();

    size_t from = (page - 1) * pageSize;
    size_t to = from + pageSize;

    if (to > cars.size())
        to = cars.size();

    if (from == 0 && cars.empty())
        return {};

    if (from >= to)
        throw NotValidPageException("page not valid");

    return CarsDTO(cars.begin() + from, cars.begin() + to);
}

CarDTO CarsFacade::GetCar(const std::string &uid)
{
    if (!m_repository)
        throw NotInitializedException("repository doesn't initilized");

    try
    {
        return m_repository->GetCar(uid);
    }
    catch (const DatabaseNotFoundException &e)
    {
        throw CarNotFoundException(e.what());
    }
}

void CarsFacade::UpdateCarAvailability(const CarAvailabilityDTO &car)
{
    if (!m_repository)
        throw NotInitializedException("repository doesn't initilized");

    try
    {
        m_repository->UpdateCarAvailability(car.carUid, car.available);
    }
    catch (const DatabaseNotFoundException &e)
    {
        throw CarNotFoundException(e.what());
    }
}
