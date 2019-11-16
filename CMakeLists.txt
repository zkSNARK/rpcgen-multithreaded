cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(A3 LANGUAGES C CXX VERSION 0.0.0)

# Specify config file.  Generate it as config.h into PROJECT_BINARY_DIR
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)
SET(CMAKE_CXX_FLAGS -pthread)
SET(CMAKE_C_FLAGS -pthread)

add_executable(client
  src/rdict_client.c
  src/rdict_clnt.c
  src/rdict_xdr.c
  )
set_target_properties(client PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(client PRIVATE -lnsl)

add_executable(server
  src/rdict_server.c
  src/rdict_svc.c
  src/rdict_xdr.c
  )
set_target_properties(server PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(server PRIVATE -lnsl)