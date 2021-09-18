cmake_minimum_required(VERSION 3.10)
project(Pogo)

add_executable(Pogo-daemon pogo-daemon.cxx pogo-daemon-api.cxx)

set_property(TARGET Pogo-daemon PROPERTY CXX_STANDARD 17)
