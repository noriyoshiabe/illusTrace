cmake_minimum_required(VERSION 3.5)

project(illustrace-filter)

set(SOURCES
  BrightnessFilter.cpp
  BlurFilter.cpp
  BinaryThresholdFilter.cpp
  ThinningFilter.cpp
  NegativeFilter.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
