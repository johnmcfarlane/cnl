set(
    MISC_FLAGS
    "-Werror -Wall -Wextra -Wpedantic -Wshadow -Wundef -ftemplate-backtrace-limit=0 -fconstexpr-backtrace-limit=0 -fconstexpr-steps=1000000000 -fdiagnostics-color=always -stdlib=libc++"
)

set(CMAKE_CXX_COMPILER_LAUNCHER "ccache")
set(CMAKE_EXE_LINKER_FLAGS "-lc++ -lc++abi")

set(EXCEPTION_ENABLED_FLAGS "-fexceptions -frtti")
set(EXCEPTION_DISABLED_FLAGS "-DBOOST_NO_EXCEPTIONS -DBOOST_NO_RTTI -fno-exceptions -fno-rtti")

# Note: enabling CNL_INT128 and CNL_SANITIZE may result in link-time failure
set(INT128_ENABLED_FLAGS "-DCNL_USE_INT128=1")
set(INT128_DISABLED_FLAGS "-DCNL_USE_INT128=0")

set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=address,undefined -fsanitize-trap=undefined -g -O0")
set(SANITIZE_ENABLED_LINKER_FLAGS "-fsanitize=address,undefined")

set(TEST_CXX_FLAGS "-Wconversion -Wno-sign-conversion -ftemplate-backtrace-limit=0")
set(SAMPLE_CXX_FLAGS "-fpermissive -Wno-sign-compare -Wno-strict-overflow")
