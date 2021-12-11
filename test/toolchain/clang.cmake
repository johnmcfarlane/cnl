include("${CMAKE_CURRENT_LIST_DIR}/posix.cmake")

set(
    CNL_CXX_FLAGS
    "${CNL_CXX_FLAGS} -fconstexpr-backtrace-limit=0 -fconstexpr-steps=1000000000 -DCNL_IMPL_ONEROUS_EVALUATION"
)
