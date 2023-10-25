#include "CarsDA.h"
#include "PostgresCarsRepository.h"

ICarsRepositoryPtr CreateCarsRepository(const IConfigPtr &conf, const std::string &connectionSection)
{
    return std::make_shared<PostgresCarsRepository>(conf, connectionSection);
}
