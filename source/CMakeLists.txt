SET(SOURCE_FILES
  noob-constant.h
  noob-value.h

  noob-value.cpp
  noob-accessor.cpp
  noob-parser.cpp
  noob-stringifier.cpp
  noob-prettifier.cpp
  )

set(LIBRARY_OUTPUT_PATH output)

add_library(NoobJson SHARED ${SOURCE_FILES})
set_target_properties(NoobJson PROPERTIES VERSION 2.1 SOVERSION 2)

install(TARGETS NoobJson LIBRARY DESTINATION lib)
install(FILES noob-constant.h noob-value.h DESTINATION include/noob-json)