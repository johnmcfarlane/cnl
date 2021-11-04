include("${CMAKE_CURRENT_LIST_DIR}/base.cmake")

set(
    MISC_FLAGS
    "${MISC_FLAGS} -ftemplate-backtrace-limit=0 -fconstexpr-backtrace-limit=0 -fconstexpr-steps=1000000000 -DCNL_IMPL_ONEROUS_EVALUATION -stdlib=libc++"
)

set(CMAKE_EXE_LINKER_FLAGS "-lc++ -lc++abi")

set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=address,undefined -fsanitize-trap=undefined -g -O0")
