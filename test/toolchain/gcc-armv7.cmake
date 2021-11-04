include("${CMAKE_CURRENT_LIST_DIR}/base.cmake")

set(
    MISC_FLAGS
    "${MISC_FLAGS} -fconcepts -fconstexpr-ops-limit=1000000000 -ftemplate-backtrace-limit=0 -pthread -Wno-psabi -DCNL_IMPL_ONEROUS_EVALUATION -march=armv7-a"
)

set(INT128_ENABLED_FLAGS "${INT128_ENABLED_FLAGS} -Wno-pedantic")

set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=address,undefined -fsanitize-undefined-trap-on-error -g -O0")
