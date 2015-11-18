include(ExternalProject)

######################################################################
# build flags

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(MISC_FLAGS "/W4 /WX /errorReport:prompt /nologo")

  set(EXCEPTION_ENABLED_FLAGS "/EHsc")
  set(EXCEPTION_DISABLED_FLAGS "")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(MISC_FLAGS "-std=c++11 -pthread -Wall -Wextra -Wfatal-errors -Werror")

  set(EXCEPTION_ENABLED_FLAGS "-fexceptions -frtti")
  set(EXCEPTION_DISABLED_FLAGS "-fno-exceptions -fno-rtti")
else ()
  message(FATAL_ERROR "unrecognized compiler: ${CMAKE_CXX_COMPILER_ID}")
endif ()

if (EXCEPTIONS)
    set(EXCEPTION_FLAGS "${EXCEPTION_ENABLED_FLAGS}")
else (EXCEPTIONS)
    set(EXCEPTION_FLAGS "${EXCEPTION_DISABLED_FLAGS}")
endif (EXCEPTIONS)

set(COMMON_CXX_FLAGS "${MISC_FLAGS} ${EXCEPTION_FLAGS}")

include_directories("${CMAKE_CURRENT_LIST_DIR}/../../include")
include_directories("${CMAKE_CURRENT_LIST_DIR}/.")

set(BUILD_SHARED_LIBS, ON)
