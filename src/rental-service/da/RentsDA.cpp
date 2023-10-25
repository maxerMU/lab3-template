#include "PostgresRentsRepository.h"
#include "RentsDA.h"

IRentsRepositoryPtr CreateRentsRepository(const IConfigPtr &conf, const std::string &connectionSection)
{
    return std::make_shared<PostgresRentsRepository>(conf, connectionSection);
}
