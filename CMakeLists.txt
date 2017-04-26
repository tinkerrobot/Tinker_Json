cmake_minimum_required(VERSION 3.7)
project(Noob_Json_Parser)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
  header/std-header.h
  header/noob-enum.h
  header/noob-value.h

  main.cpp
  source/noob-value.cpp
  )
add_executable(Noob_Json_Parser ${SOURCE_FILES})