include("${CMAKE_CURRENT_LIST_DIR}/gcc.cmake")

# -Warray-bounds and -Wstringop-overread produce false positives in GCC-12
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=103483
set(
    CNL_CXX_FLAGS
    "${CNL_CXX_FLAGS} -Wno-array-bounds -Wno-stringop-overread"
)
