set(
    MISC_FLAGS
    "-Werror -Wall -Wextra -Wpedantic -Wshadow -Wundef -ftemplate-backtrace-limit=0 -fconstexpr-backtrace-limit=0 -fconstexpr-steps=1000000000 -stdlib=libstdc++"
)

set(EXCEPTION_ENABLED_FLAGS "-fexceptions -frtti")
set(EXCEPTION_DISABLED_FLAGS "-DBOOST_NO_EXCEPTIONS -DBOOST_NO_RTTI -fno-exceptions -fno-rtti")

set(INT128_ENABLED_FLAGS "-DCNL_USE_INT128=1")
set(INT128_DISABLED_FLAGS "-DCNL_USE_INT128=0")

set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=address,undefined -fsanitize-trap=undefined -g -O0")
set(SANITIZE_ENABLED_LINKER_FLAGS "-fsanitize=address,undefined -rtlib=compiler-rt -lgcc_s")

set(TEST_CXX_FLAGS "-Wconversion -Wno-sign-conversion -ftemplate-backtrace-limit=0")
set(SAMPLE_CXX_FLAGS "-fpermissive -Wno-sign-compare -Wno-strict-overflow")
