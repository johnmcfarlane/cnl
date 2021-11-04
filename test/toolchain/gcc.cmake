include("${CMAKE_CURRENT_LIST_DIR}/base.cmake")

set(
    MISC_FLAGS
    "${MISC_FLAGS} -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -Wno-psabi -fconcepts -fconstexpr-ops-limit=1000000000 -ftemplate-backtrace-limit=0 -pthread"
)

set(INT128_ENABLED_FLAGS "${INT128_ENABLED_FLAGS} -Wno-pedantic")

set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=address,undefined -fsanitize-undefined-trap-on-error -g -O0")
