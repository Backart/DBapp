# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "Bcryptcpp\\CMakeFiles\\bcryptcpp_autogen.dir\\AutogenUsed.txt"
  "Bcryptcpp\\CMakeFiles\\bcryptcpp_autogen.dir\\ParseCache.txt"
  "Bcryptcpp\\bcryptcpp_autogen"
  "CMakeFiles\\SqlLiteBDApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SqlLiteBDApp_autogen.dir\\ParseCache.txt"
  "SqlLiteBDApp_autogen"
  )
endif()
