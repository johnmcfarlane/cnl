set(
    MISC_FLAGS
    "-Werror -Wall -Wextra -Wno-psabi -Wpedantic -Wshadow -Wundef -fconcepts -fconstexpr-ops-limit=134217728 -fdiagnostics-color=always -ftemplate-backtrace-limit=0 -pthread"
)

set(CMAKE_CXX_COMPILER_LAUNCHER "ccache")

set(EXCEPTION_ENABLED_FLAGS "-fexceptions -frtti")
set(EXCEPTION_DISABLED_FLAGS "-DBOOST_NO_EXCEPTIONS -DBOOST_NO_RTTI -fno-exceptions -fno-rtti")

set(INT128_ENABLED_FLAGS "-DCNL_USE_INT128=1 -Wno-pedantic")
set(INT128_DISABLED_FLAGS "-DCNL_USE_INT128=0")

set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=address,undefined -fsanitize-undefined-trap-on-error -g -O0")
set(SANITIZE_ENABLED_LINKER_FLAGS "-fsanitize=address,undefined")

set(TEST_CXX_FLAGS "-Wconversion -Wno-sign-conversion -ftemplate-backtrace-limit=0")
set(SAMPLE_CXX_FLAGS "-fpermissive -Wno-sign-compare -Wno-strict-overflow")
