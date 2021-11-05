include("${CMAKE_CURRENT_LIST_DIR}/base.cmake")

set(
    MISC_FLAGS
    "${MISC_FLAGS} -fconstexpr-backtrace-limit=0 -fconstexpr-steps=1000000000 -DCNL_IMPL_ONEROUS_EVALUATION"
)

set(SANITIZE_ENABLED_CXX_FLAGS "${SANITIZE_ENABLED_CXX_FLAGS} -fsanitize-trap=undefined")
