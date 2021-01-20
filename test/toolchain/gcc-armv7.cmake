set(
    MISC_FLAGS
    "-Werror -Wall -Wextra -Wpedantic -Wshadow -Wundef -fconcepts -ftemplate-backtrace-limit=0 -pthread -Wno-psabi -march=armv7-a"
)

set(EXCEPTION_ENABLED_FLAGS "-fexceptions -frtti")
set(EXCEPTION_DISABLED_FLAGS "-DBOOST_NO_EXCEPTIONS -DBOOST_NO_RTTI -fno-exceptions -fno-rtti")

set(INT128_ENABLED_FLAGS "-DCNL_USE_INT128=1 -Wno-pedantic")
set(INT128_DISABLED_FLAGS "-DCNL_USE_INT128=0")

set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=address,undefined -fsanitize-undefined-trap-on-error -g -O0")
set(SANITIZE_ENABLED_LINKER_FLAGS "-fsanitize=address,undefined")

set(TEST_CXX_FLAGS "-Wconversion -Wno-sign-conversion -ftemplate-backtrace-limit=0")
set(SAMPLE_CXX_FLAGS "-fpermissive -Wno-sign-compare -Wno-strict-overflow")
