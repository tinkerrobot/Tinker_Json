cmake_minimum_required(VERSION 3.7)
project(Noob_Json_Parser)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
  header/noob-constant.h
  header/noob-value.h

  source/noob-value.cpp
  source/noob-accessor.cpp

  main.cpp
  )
add_executable(Noob_Json_Parser ${SOURCE_FILES})