CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(Noob_Json_Parser)

SET(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

SET(SOURCE_FILES
  header/noob-constant.h
  header/noob-value.h

  source/noob-value.cpp
  source/noob-accessor.cpp
  source/noob-parser.cpp

  test.cpp
  source/noob-stringifier.cpp)
ADD_EXECUTABLE(Noob_Json_Parser ${SOURCE_FILES})
