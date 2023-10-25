project( "base" CXX )

add_subdirectory(logger_test)

add_library( ${PROJECT_NAME} STATIC )

target_sources( ${PROJECT_NAME} PRIVATE
    external_includes/exceptions/base_exception.h
    external_includes/exceptions/database_exceptions.h
    external_includes/exceptions/logic_exceptions.h
    external_includes/exceptions/server_exceptions.h
    external_includes/config/IConfig.h
    external_includes/config/config_creators.h
    external_includes/config/base_sections.h
    external_includes/logger/ILogger.h
    external_includes/logger/LoggerFactory.h

    config/yamlcpp_config.h
    config/yamlcpp_config.cpp
    config/config_creators.cpp

    logger/SpdLogger.h
    logger/SpdLogger.cpp
    logger/LoggerFactory.cpp
)

target_link_libraries( ${PROJECT_NAME}
    PUBLIC
        yaml-cpp
        spdlog::spdlog
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external_includes )
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )