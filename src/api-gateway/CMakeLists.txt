project( api-gateway CXX )

add_executable( ${PROJECT_NAME} )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20 )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/api-gateway.yaml ${CMAKE_CURRENT_BINARY_DIR}/api-gateway.yaml)

target_sources( ${PROJECT_NAME} PRIVATE
  main.cpp

  ApiGatewayHandler.h
  ApiGatewayHandler.cpp

  context/ApiGatewayContext.h
  context/ApiGatewayContext.cpp

  dto/CarDTO.h
  dto/CarDTO.cpp
  dto/GetRentDTO.h
  dto/GetRentDTO.cpp
  dto/PaymentDTO.h
  dto/PaymentDTO.cpp
  dto/PostRentCarDTO.h
  dto/PostRentCarDTO.cpp
  dto/PostRentDTO.h
  dto/PostRentDTO.cpp
  dto/PostRentResultDTO.h
  dto/PostRentResultDTO.cpp
  dto/PostPaymentDTO.h
  dto/PostPaymentDTO.cpp
  dto/RentDTO.h
  dto/RentDTO.cpp
  dto/CarAvailabilityDTO.h
  dto/CarAvailabilityDTO.cpp

  routes/HealthRoute.h
  routes/HealthRoute.cpp
  routes/GetRentPrep.h
  routes/GetRentPrep.cpp
  routes/GetRentsPrep.h
  routes/GetRentsPrep.cpp
  routes/GetRentPost.h
  routes/GetRentPost.cpp
  routes/GetRentsPost.h
  routes/GetRentsPost.cpp
  routes/GetCarRoute.h
  routes/GetCarRoute.cpp
  routes/GetCarsRoute.h
  routes/GetCarsRoute.cpp
  routes/GetPaymentRoute.h
  routes/GetPaymentRoute.cpp
  routes/GetRentRoute.h
  routes/GetRentRoute.cpp
  routes/GetRentsRoute.h
  routes/GetRentsRoute.cpp
  routes/PostRentPrep.h
  routes/PostRentPrep.cpp
  routes/PostRentPost.h
  routes/PostRentPost.cpp
  routes/PostPaymentRoute.h
  routes/PostPaymentRoute.cpp
  routes/PostRentRoute.h
  routes/PostRentRoute.cpp
  routes/FinishRentPrep.h
  routes/FinishRentPrep.cpp
  routes/FinishRentRoute.h
  routes/FinishRentRoute.cpp
  routes/UpdateCarAvailabilityRoute.h
  routes/UpdateCarAvailabilityRoute.cpp
  routes/CancelRentPrep.h
  routes/CancelRentPrep.cpp
  routes/CancelRentRoute.h
  routes/CancelRentRoute.cpp
  routes/CancelPaymentRoute.h
  routes/CancelPaymentRoute.cpp
)

target_include_directories(${PROJECT_NAME} 
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        base
        NetLib
        jsoncpp_lib
)