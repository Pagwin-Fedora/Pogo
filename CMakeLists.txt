cmake_minimum_required(VERSION 3.10)
project(Pogo)

include(ExternalProject)


# rust tcp fronteTnd build stuff
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/tcp_frontend)
ExternalProject_Add(
    tcp_frontend
    DOWNLOAD_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cargo build COMMAND cargo build --release
    BINARY_DIR "${CMAKE_SOURCE_DIR}/common-rust"
    INSTALL_COMMAND ""
    LOG_BUILD ON)
add_dependencies(TCP_Frontend tcp_frontend)
target_link_libraries(TCP_Frontend
    debug "${CMAKE_SOURCE_DIR}/common-rust/target/debug/libtcp_frontend.a"
    optimized "${CMAKE_SOURCE_DIR}/common-rust/target/release/libtcp_frontend.a"
    )


add_executable(Pogo-daemon pogo-daemon.cxx pogo-daemon-api.cxx TCP_Frontend)

set_property(TARGET Pogo-daemon PROPERTY CXX_STANDARD 17)
