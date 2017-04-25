cmake_minimum_required(VERSION 3.7)
project(Noob_Json_Parser)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
  header/std-header.h

  main.cpp
  source/test-function.cpp
  )
add_executable(Noob_Json_Parser ${SOURCE_FILES})