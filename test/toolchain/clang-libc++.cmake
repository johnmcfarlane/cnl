include("${CMAKE_CURRENT_LIST_DIR}/clang.cmake")

set(CNL_CXX_FLAGS "${CNL_CXX_FLAGS} -stdlib=libc++")
set(CNL_EXE_LINKER_FLAGS "-lc++ -lc++abi")
