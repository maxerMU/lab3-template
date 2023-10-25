project( persons_ut CXX )

add_executable( ${PROJECT_NAME} )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20 )

target_sources( ${PROJECT_NAME} PRIVATE
    main.cpp
    PersonsFacadeTest.h
    PersonsFacadeTest.cpp
    PersonsRepositoryMock.h

    ../bl/PersonsFacade.h
    ../bl/PersonsFacade.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        base
        NetLib
        gmock
        GTest::GTest
        GTest::Main
)

add_test(NAME persons_ut COMMAND ${PROJECT_NAME})
