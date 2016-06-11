cmake_minimum_required(VERSION 3.5)

project(illustrace-core)

set(SOURCES
  Illustrace.cpp
  BrightnessFilter.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
