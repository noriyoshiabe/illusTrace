cmake_minimum_required(VERSION 3.5)

project(illustrace-core)

set(SOURCES
  Illustrace.cpp
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ../filter
)

add_library(${PROJECT_NAME} ${SOURCES})
