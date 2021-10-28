cmake_minimum_required(VERSION 3.10)
project(Pogo-daemon-postgres-backend)

set(TARGET backend_postgres)
set(SRCS postgres_backend.go)
set(LIB postgres_backend.so)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${LIB}
  DEPENDS ${SRCS}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND env GOPATH=${GOPATH} go build -buildmode=c-archive
  -o "${CMAKE_CURRENT_BINARY_DIR}/${LIB}"
  ${CMAKE_GO_FLAGS} ./...
  COMMENT "Building postgres backend")

add_custom_target(${TARGET} DEPENDS ${LIB} ${HEADER})
add_library(postgres_backend STATIC IMPORTED GLOBAL)
add_dependencies(postgres_backend ${TARGET})
set_target_properties(postgres_backend
  PROPERTIES
  IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/${LIB}
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR})
