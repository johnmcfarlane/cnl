include("${CMAKE_CURRENT_LIST_DIR}/clang.cmake")

set(MISC_FLAGS "${MISC_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "-lc++ -lc++abi")
