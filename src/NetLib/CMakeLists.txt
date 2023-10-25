project( NetLib CXX )

add_subdirectory( demo )

add_library( ${PROJECT_NAME} STATIC )

target_compile_options( ${PROJECT_NAME} PUBLIC -fcoroutines)

target_sources( ${PROJECT_NAME} PRIVATE
    net.cpp

    base/beast_req.cpp
    base/beast_req.h
    base/beast_resp.cpp
    base/beast_resp.h
    base/IResponseFactory.cpp
    base/BeastResponseFactory.cpp
    base/BeastResponseFactory.h

    connections/client_server_connection.cpp
    connections/client_server_connection.h
    connections/server_connection.cpp
    connections/server_connection.h

    coroutines_support/read_awaiter.cpp
    coroutines_support/read_awaiter.h
    coroutines_support/http_read_awaiter.cpp
    coroutines_support/http_read_awaiter.h
    coroutines_support/http_write_awaiter.cpp
    coroutines_support/http_write_awaiter.h
    coroutines_support/std_future.hpp
    coroutines_support/write_awaiter.cpp
    coroutines_support/write_awaiter.h

    router/router.cpp
    router/RequestHandlerContext.h
    router/RequestHandlerContext.cpp

    sessions/echo_session.cpp
    sessions/echo_session.h
    sessions/http_client_server_session.cpp
    sessions/http_client_server_session.h
    sessions/http_session.cpp
    sessions/http_session.h

    external_includes/reqresp/IResponseFactory.h
    external_includes/reqresp/reqresp.h
    external_includes/request_handlers/IClientServerReqHandler.h
    external_includes/request_handlers/IServerReqHandler.h
    external_includes/router/IClientServerRoute.h
    external_includes/router/IRequestHandlerContext.h
    external_includes/router/router.h
    external_includes/network/net.h
    external_includes/network/net_asio.h
    external_includes/network/IConnection.h
    external_includes/network/IServerSession.h
    external_includes/network/IClientServerSession.h
)

target_include_directories( ${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/base/
    ${CMAKE_CURRENT_SOURCE_DIR}/coroutines_support/
    ${CMAKE_CURRENT_SOURCE_DIR}/sessions/
    ${CMAKE_CURRENT_SOURCE_DIR}/connections/
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external_includes
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        base
        Boost::boost
)