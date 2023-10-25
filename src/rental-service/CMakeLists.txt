project( rental-service CXX )

add_executable( ${PROJECT_NAME} )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20 )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rents.yaml ${CMAKE_CURRENT_BINARY_DIR}/rents.yaml)

target_sources( ${PROJECT_NAME} PRIVATE
    main.cpp

    dto/RentDTO.h
    dto/RentDTO.cpp
    dto/PostRentDTO.h
    dto/PostRentDTO.cpp

    gateway/RentsServerHandler.h
    gateway/RentsServerHandler.cpp
    gateway/RentsGateway.h
    gateway/RentsGateway.cpp

    bl/RentsFacade.h
    bl/RentsFacade.cpp

    da/IRentsRepository.h
    da/RentsDA.h
    da/RentsDA.cpp
    da/PostgresRentsRepository.h
    da/PostgresRentsRepository.cpp
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
