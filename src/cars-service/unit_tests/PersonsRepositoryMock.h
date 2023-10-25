#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "da/IPersonsRepository.h"

class PersonsRepositoryMock : public IPersonsRepository
{
public:
    MOCK_METHOD(void, AddPerson, (const PersonPostDTO& person), (override));
    MOCK_METHOD(PersonsDTO, GetPersons, (), (override));
    MOCK_METHOD(PersonDTO, GetPerson, (size_t id), (override));
    MOCK_METHOD(PersonDTO, PatchPerson, (size_t id, const PersonPatchDTO& person), (override));
    MOCK_METHOD(void, DeletePerson, (size_t id), (override));
};
