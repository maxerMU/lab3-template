project( cars-service CXX )

# add_subdirectory(unit_tests)

add_executable( ${PROJECT_NAME} )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20 )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cars.yaml ${CMAKE_CURRENT_BINARY_DIR}/cars.yaml)

target_sources( ${PROJECT_NAME} PRIVATE
    main.cpp

    dto/CarDTO.h
    dto/CarDTO.cpp
    dto/CarAvailabilityDTO.h
    dto/CarAvailabilityDTO.cpp

    gateway/CarsServerHandler.h
    gateway/CarsServerHandler.cpp
    gateway/CarsGateway.h
    gateway/CarsGateway.cpp

    bl/CarsFacade.h
    bl/CarsFacade.cpp

    da/ICarsRepository.h
    da/CarsDA.h
    da/CarsDA.cpp
    da/PostgresCarsRepository.h
    da/PostgresCarsRepository.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        base
        NetLib
        jsoncpp_lib
        pqxx
)
