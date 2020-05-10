include(cmake/common.cmake)

include(ExternalProject)

add_custom_target(test-all)

add_subdirectory(benchmark)
add_subdirectory(unit)
