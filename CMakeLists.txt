cmake_minimum_required(VERSION 3.10)
project(Pogo)

include(ExternalProject)
add_link_options(-lpthread -ldl)
add_executable(Pogo-daemon pogo-daemon.cxx pogo-daemon-api.cxx)


# rust tcp fronteTnd build stuff
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/Rust)
ExternalProject_Add(
    tcp_frontend
    DOWNLOAD_COMMAND ""
    CONFIGURE_COMMAND ""
    BINARY_DIR "${CMAKE_SOURCE_DIR}/tcp_frontend"
    BUILD_COMMAND cargo build COMMAND cargo build --release
    INSTALL_COMMAND ""
    LOG_BUILD ON
)
add_dependencies(Pogo-daemon tcp_frontend)
target_link_libraries(Pogo-daemon
    debug "${CMAKE_SOURCE_DIR}/tcp_frontend/target/debug/libtcp_frontend.a"
    optimized "${CMAKE_SOURCE_DIR}/tcp_frontend/target/release/libtcp_frontend.a"
    )

#set C++ standard to the standard needed
set_property(TARGET Pogo-daemon PROPERTY CXX_STANDARD 17)
