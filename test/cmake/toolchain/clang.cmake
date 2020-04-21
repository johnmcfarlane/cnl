set(
    MISC_FLAGS
    "-Werror -Wall -Wextra -Wpedantic -Wundef -ftemplate-backtrace-limit=0 -fconstexpr-backtrace-limit=0 -fconstexpr-steps=1000000000"
)

set(EXCEPTION_ENABLED_FLAGS "-fexceptions -frtti")
set(EXCEPTION_DISABLED_FLAGS "-DBOOST_NO_EXCEPTIONS -DBOOST_NO_RTTI -fno-exceptions -fno-rtti")

set(INT128_ENABLED_FLAGS "-DCNL_USE_INT128=1")
set(INT128_DISABLED_FLAGS "-DCNL_USE_INT128=0")

set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=address,undefined -fsanitize-trap=undefined -g -O0")
set(SANITIZE_ENABLED_LINKER_FLAGS "-fsanitize=address,undefined -rtlib=compiler-rt -lgcc_s")
