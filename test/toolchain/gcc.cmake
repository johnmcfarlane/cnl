include("${CMAKE_CURRENT_LIST_DIR}/posix.cmake")

set(
    CNL_CXX_FLAGS
    "${CNL_CXX_FLAGS} -Wno-psabi -fconcepts -fconstexpr-ops-limit=1000000000"
)
set(CNL_CXX_FLAGS_DEBUG "-D_GLIBCXX_ASSERTIONS")

set(INT128_ENABLED_FLAGS "${INT128_ENABLED_FLAGS} -Wno-pedantic")
