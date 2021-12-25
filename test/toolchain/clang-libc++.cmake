include("${CMAKE_CURRENT_LIST_DIR}/clang.cmake")

set(CNL_CXX_FLAGS "${CNL_CXX_FLAGS} -stdlib=libc++")
set(CNL_EXE_LINKER_FLAGS "-lc++ -lc++abi")

# https://github.com/google/benchmark/issues/351#issuecomment-285772578
set(HAVE_GNU_POSIX_REGEX 0)
set(HAVE_POSIX_REGEX 0)
set(HAVE_STD_REGEX 0)
