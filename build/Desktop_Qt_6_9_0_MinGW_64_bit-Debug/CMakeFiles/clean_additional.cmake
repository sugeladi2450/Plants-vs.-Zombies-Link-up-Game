# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\QLinkTest_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QLinkTest_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\QLink_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QLink_autogen.dir\\ParseCache.txt"
  "QLinkTest_autogen"
  "QLink_autogen"
  )
endif()
