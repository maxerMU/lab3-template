project( net_lib_demo_client_server CXX )

add_executable( ${PROJECT_NAME} )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20 )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.yaml ${CMAKE_CURRENT_BINARY_DIR}/config.yaml)

target_sources( ${PROJECT_NAME} PRIVATE
  main.cpp
  DummyClientServerHandler.h
  DummyClientServerRoute.h
  DummyClientServerHandler.cpp
  DummyClientServerRoute.cpp
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        base
        NetLib
)