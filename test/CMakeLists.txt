include(cmake/install-conan.cmake)
include(cmake/common.cmake)

include(ExternalProject)

if (NOT DEFINED MISC_FLAGS)
  message(FATAL_ERROR "Please provide CMAKE_TOOLCHAIN_FILE")
endif ()

add_custom_target(test-all)

add_subdirectory(benchmark)
add_subdirectory(unit)
