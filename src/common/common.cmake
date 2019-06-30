﻿include(ExternalProject)

######################################################################
# build flags

string(COMPARE EQUAL "${CMAKE_CXX_COMPILER_ID}" "AppleClang" IS_APPLECLANG)
string(COMPARE EQUAL "${CMAKE_CXX_COMPILER_ID}" "Clang" IS_CLANG)
string(COMPARE EQUAL "${CMAKE_CXX_COMPILER_ID}" "GNU" IS_GCC_FAMILY)
string(COMPARE EQUAL "${CMAKE_CXX_COMPILER_ID}" "MSVC" IS_MSVC)

if (IS_CLANG OR IS_APPLECLANG)
    set(IS_CLANG_FAMILY 1)
else ()
    set(IS_CLANG_FAMILY 0)
endif ()

if (IS_MSVC)
  # https://developercommunity.visualstudio.com/content/problem/55671/c4307-issued-for-unsigned.html
  set(MISC_FLAGS "/W4 /WX /errorReport:prompt /nologo /wd4307")

  # not tested
  set(CPP17_ENABLED_FLAGS "/std:c++17")
  set(CPP20_ENABLED_FLAGS "/std:latest")

  set(EXCEPTION_ENABLED_FLAGS "/GR /EHsc")
  set(EXCEPTION_DISABLED_FLAGS "/GR- -DBOOST_NO_EXCEPTIONS -DBOOST_NO_RTTI")

  # 128-bit integers are not supported in MSVC
  set(INT128_ENABLED_FLAGS "")
  set(INT128_DISABLED_FLAGS "")
elseif (IS_CLANG_FAMILY OR IS_GCC_FAMILY)
  set(MISC_FLAGS "-Wall -Wextra -Werror -Wundef -ftemplate-backtrace-limit=0")

  if (NOT IS_APPLECLANG)
      string(APPEND MISC_FLAGS " -pthread")
  endif ()

  if (IS_CLANG)
      string(APPEND MISC_FLAGS " -fconstexpr-backtrace-limit=0 -fconstexpr-steps=1000000000")
      set(CPP20_ENABLED_FLAGS "-std=c++2a")
  else ()
      string(APPEND MISC_FLAGS " -Wno-psabi")
      set(CPP20_ENABLED_FLAGS "-std=c++2a -fconcepts")
  endif ()

  set(CPP17_ENABLED_FLAGS "-std=c++17")

  set(EXCEPTION_ENABLED_FLAGS "-fexceptions -frtti")
  set(EXCEPTION_DISABLED_FLAGS "-DBOOST_NO_EXCEPTIONS -DBOOST_NO_RTTI -fno-exceptions -fno-rtti")

  set(INT128_ENABLED_FLAGS "-DCNL_USE_INT128=1")
  set(INT128_DISABLED_FLAGS "-DCNL_USE_INT128=0 -Wpedantic")
else ()
  message(FATAL_ERROR "unrecognized compiler: ${CMAKE_CXX_COMPILER_ID}")
endif ()

set(CNL_STD 17 CACHE STRING "version of C++ standard: 11, 14, 17 or 20")
if (${CNL_STD} STREQUAL "20")
    set(STD_FLAGS "${CPP20_ENABLED_FLAGS}")
elseif (${CNL_STD} STREQUAL "17")
    set(STD_FLAGS "${CPP17_ENABLED_FLAGS}")
else ()
    set(CMAKE_CXX_STANDARD ${CNL_STD})
endif ()

set(CNL_EXCEPTIONS ON CACHE BOOL "compile with exceptions enabled")
if (CNL_EXCEPTIONS)
    set(EXCEPTION_FLAGS "${EXCEPTION_ENABLED_FLAGS}")
else (CNL_EXCEPTIONS)
    set(EXCEPTION_FLAGS "${EXCEPTION_DISABLED_FLAGS}")
endif (CNL_EXCEPTIONS)

set(CNL_INT128 ON CACHE BOOL "compile with support for 128-bit integers (where available)")
if (CNL_INT128)
    set(INT128_FLAGS "${INT128_ENABLED_FLAGS}")
else (CNL_INT128)
    set(INT128_FLAGS "${INT128_DISABLED_FLAGS}")
endif (CNL_INT128)

set(COMMON_CXX_FLAGS "${MISC_FLAGS} ${STD_FLAGS} ${EXCEPTION_FLAGS} ${INT128_FLAGS}")

set(BUILD_SHARED_LIBS, ON)
