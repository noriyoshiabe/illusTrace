cmake_minimum_required(VERSION 3.5)

project(illustrace-cli)
set(TARGET_NAME illustrace)

set(SOURCES
  main.cpp
  CLI.cpp
)

add_executable(${TARGET_NAME} ${SOURCES})
