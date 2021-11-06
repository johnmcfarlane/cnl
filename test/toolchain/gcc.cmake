include("${CMAKE_CURRENT_LIST_DIR}/base.cmake")

set(
    CNL_CXX_FLAGS
    "${CNL_CXX_FLAGS} -D_GLIBCXX_ASSERTIONS -Wno-psabi -fconcepts -fconstexpr-ops-limit=1000000000"
)

set(INT128_ENABLED_FLAGS "${INT128_ENABLED_FLAGS} -Wno-pedantic")
