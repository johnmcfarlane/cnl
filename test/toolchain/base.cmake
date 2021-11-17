# Note: CMAKE_CXX_FLAGS_INIT wont work here.
# If CMAKE_CXX_FLAGS is passed in on command line,
# CMAKE_CXX_FLAGS_INIT is ignored.
set(
    CNL_CXX_FLAGS
    "-Werror -Wall -Wextra -Wshadow -Wundef -fdiagnostics-color=always -ftemplate-backtrace-limit=0"
)

set(CMAKE_CXX_COMPILER_LAUNCHER "ccache")

set(EXCEPTION_ENABLED_FLAGS "-fexceptions -frtti")
set(EXCEPTION_DISABLED_FLAGS "-DBOOST_NO_EXCEPTIONS -DBOOST_NO_RTTI -fno-exceptions -fno-rtti")

# Note: enabling CNL_INT128 and CNL_SANITIZE for Clang may result in link-time failure.
# Adding linker flags `-rtlib=compiler-rt` (plus `-lgcc_s` on Linux) helps but is not portable.
set(INT128_ENABLED_FLAGS "-DCNL_USE_INT128=1")
set(INT128_DISABLED_FLAGS "-DCNL_USE_INT128=0 -Wpedantic")

set(SANITIZE_ENABLED_CXX_FLAGS "-fsanitize=address,undefined -fno-sanitize-recover=all")
set(SANITIZE_ENABLED_LINKER_FLAGS "-fsanitize=address,undefined")

set(TEST_CXX_FLAGS "-Wconversion -Wno-sign-conversion")
set(SAMPLE_CXX_FLAGS "-fpermissive -Wno-sign-compare -Wno-strict-overflow")
