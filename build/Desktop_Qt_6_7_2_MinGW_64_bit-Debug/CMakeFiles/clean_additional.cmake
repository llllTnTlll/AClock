# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appAClock_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appAClock_autogen.dir\\ParseCache.txt"
  "appAClock_autogen"
  )
endif()
