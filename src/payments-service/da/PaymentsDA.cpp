#include "PaymentsDA.h"
#include "PostgresPaymentsRepository.h"

IPaymentsRepositoryPtr CreatePaymentsRepository(const IConfigPtr &conf, const std::string &connectionSection)
{
    return std::make_shared<PostgresPaymentsRepository>(conf, connectionSection);
}
