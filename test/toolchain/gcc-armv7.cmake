include("${CMAKE_CURRENT_LIST_DIR}/gcc.cmake")

set(CMAKE_C_COMPILER arm-linux-gnueabi-gcc-10)
set(CMAKE_CXX_COMPILER arm-linux-gnueabi-g++-10)

set(CNL_CXX_FLAGS "${CNL_CXX_FLAGS} -DCNL_IMPL_ONEROUS_EVALUATION -march=armv7-a")
