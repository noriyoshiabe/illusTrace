cmake_minimum_required(VERSION 3.5)

project(illustrace-cli)
set(TARGET_NAME illustrace)

add_definitions(-Wall)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
  add_definitions(-DDEBUG)
endif ()

add_subdirectory (../core ${CMAKE_CURRENT_BINARY_DIR}/core)

set(SOURCES
  main.cpp
  CLI.cpp
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ../core
)

add_executable(${TARGET_NAME} ${SOURCES})
target_link_libraries(${TARGET_NAME} illustrace-core)
