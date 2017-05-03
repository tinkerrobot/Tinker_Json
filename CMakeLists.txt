PROJECT(Noob_Json_Parser)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

SET(SOURCE_FILES
  header/noob-constant.h
  header/noob-value.h

  source/noob-value.cpp

  test.cpp
  )
ADD_EXECUTABLE(Noob_Json_Parser ${SOURCE_FILES})
