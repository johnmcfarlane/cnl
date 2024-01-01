# test framework

add_library(cnl_test INTERFACE)
target_include_directories(cnl_test INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(cnl_test INTERFACE ${CNL_GTEST_MAIN_TARGET})
