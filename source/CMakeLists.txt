SET(SOURCE_FILES
  TinkerConstant.h
  TinkerValue.h

  TinkerValue.cpp
  TinkerAccessor.cpp
  TinkerParser.cpp
  TinkerStringifier.cpp
  TinkerPrettifier.cpp
  )

set(LIBRARY_OUTPUT_PATH output)

add_library(TinkerJson SHARED ${SOURCE_FILES})
set_target_properties(TinkerJson PROPERTIES VERSION 3.0 SOVERSION 3)

install(TARGETS TinkerJson LIBRARY DESTINATION lib)
install(FILES TinkerConstant.h TinkerValue.h DESTINATION include/tinker-json)