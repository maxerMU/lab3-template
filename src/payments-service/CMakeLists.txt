project( payments-service CXX )

add_executable( ${PROJECT_NAME} )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20 )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/payments.yaml ${CMAKE_CURRENT_BINARY_DIR}/payments.yaml)

target_sources( ${PROJECT_NAME} PRIVATE
    main.cpp

    dto/PaymentDTO.h
    dto/PaymentDTO.cpp
    dto/PostPaymentDTO.h
    dto/PostPaymentDTO.cpp

    gateway/PaymentsServerHandler.h
    gateway/PaymentsServerHandler.cpp
    gateway/PaymentsGateway.h
    gateway/PaymentsGateway.cpp

    bl/PaymentsFacade.h
    bl/PaymentsFacade.cpp

    da/IPaymentsRepository.h
    da/PaymentsDA.h
    da/PaymentsDA.cpp
    da/PostgresPaymentsRepository.h
    da/PostgresPaymentsRepository.cpp
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
