project( net_lib_demo_server CXX )

add_executable( ${PROJECT_NAME} )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20 )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/demo.yaml ${CMAKE_CURRENT_BINARY_DIR}/demo.yaml)

target_sources( ${PROJECT_NAME} PRIVATE
  main.cpp
  dummy_server_handler.h
  dummy_server_handler.cpp
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        base
        NetLib
)
