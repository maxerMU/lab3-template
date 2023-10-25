## THIS PROJECT JUST SHOWS LOGGER USAGE
## AND SHOULD BE DELETED WHEN THERE WILL
## BE ENOUGH EXAMPLES IN CODE

project( "logger_test" CXX )

add_executable( ${PROJECT_NAME} )

target_sources( ${PROJECT_NAME} PRIVATE
    main.cpp
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        base
)
