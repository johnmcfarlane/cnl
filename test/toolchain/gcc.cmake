include("${CMAKE_CURRENT_LIST_DIR}/base.cmake")

set(
    MISC_FLAGS
    "${MISC_FLAGS} -D_GLIBCXX_ASSERTIONS -Wno-psabi -fconcepts -fconstexpr-ops-limit=1000000000 -pthread"
)

set(INT128_ENABLED_FLAGS "${INT128_ENABLED_FLAGS} -Wno-pedantic")

set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=address,undefined -fsanitize-undefined-trap-on-error")
