#include "PersonsFacadeTest.h"

#include <cmath>
#include "bl/PersonsFacade.h"

using ::testing::_;
using ::testing::Return;

void PersonsFacadeTests::SetUp()
{
    m_personsDb = std::make_shared<PersonsRepositoryMock>();

    PersonsFacade::Instance()->Init(m_personsDb);
}

void PersonsFacadeTests::TearDown()
{
}

TEST_F(PersonsFacadeTests, create_person)
{
    PersonPostDTO person{"name", "address", "work", 1};
    EXPECT_CALL(*m_personsDb, AddPerson(person)).Times(1);

    PersonsFacade::Instance()->AddPerson(person);
}

TEST_F(PersonsFacadeTests, read_person)
{
    PersonDTO person{10, "name", "address", "work", 1};
    EXPECT_CALL(*m_personsDb, GetPerson(person.id)).Times(1).WillRepeatedly(Return(person));

    PersonDTO res = PersonsFacade::Instance()->GetPerson(person.id);

    EXPECT_TRUE(res == person);
}

TEST_F(PersonsFacadeTests, read_persons)
{
    PersonsDTO persons({{10, "name", "address", "work", 1}, {11, "name 1", "address 1", "work 1", 10}});
    EXPECT_CALL(*m_personsDb, GetPersons()).Times(1).WillRepeatedly(Return(persons));

    PersonsDTO res = PersonsFacade::Instance()->GetPersons();

    EXPECT_TRUE(res == persons);
}

TEST_F(PersonsFacadeTests, delete_person)
{
    size_t personId = 1;
    EXPECT_CALL(*m_personsDb, DeletePerson(personId)).Times(personId);
    PersonsFacade::Instance()->DeletePerson(personId);
}