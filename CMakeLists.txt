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
  source/noob-stringifier.cpp
  source/noob-prettifier.cpp

  test.cpp
  )
ADD_EXECUTABLE(Noob_Json_Parser ${SOURCE_FILES})
