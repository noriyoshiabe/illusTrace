cmake_minimum_required(VERSION 3.5)

project(illustrace-core)

set(SOURCES
  Illustrace.cpp
  Filter.cpp
  CenterLineBuilder.cpp
  BezierSplineBuilder.cpp
  FeatureDetector.cpp
  GraphBuilder.cpp
  Log.cpp
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(${PROJECT_NAME} ${SOURCES})
