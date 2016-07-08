cmake_minimum_required(VERSION 3.5)

project(illustrace-core)

set(SOURCES
  Illustrace.cpp
  Document.cpp
  Filter.cpp
  BezierSplineBuilder.cpp
  osx/PaintMaskBuilder.cpp
  SVGWriter.cpp
  Editor.cpp
  Log.cpp
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIBXML2_INCLUDE_DIR}
)

add_library(${PROJECT_NAME} ${SOURCES})
