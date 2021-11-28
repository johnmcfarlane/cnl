include("${CMAKE_CURRENT_LIST_DIR}/gcc.cmake")

set(
    CNL_CXX_FLAGS
    "${CNL_CXX_FLAGS} -Wno-array-bounds -Wno-stringop-overread"
)
