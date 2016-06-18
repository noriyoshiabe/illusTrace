cmake_minimum_required(VERSION 3.5)

project(illustrace-lib)

set(SOURCES
  Log.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
