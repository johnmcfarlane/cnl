include(ExternalProject)

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

set(COMMON_CXX_FLAGS "${MISC_FLAGS} ${EXCEPTION_FLAGS} ${INT128_FLAGS}")
