cmake_minimum_required(VERSION 2.8.4)
include("${CMAKE_CURRENT_LIST_DIR}/../common/common.cmake")

# /ZI /Gm  /Od /Fd"x64\Debug\vc140.pdb" /D "_DEBUG" /RTC1 /Gd /MTd /Fa"x64\Debug\" /Fo"x64\Debug\" /Fp"x64\Debug\fixed_point_test.pch"
# /GL /Gy /Zi /Gm- /O2 /Fd"x64\Release\vc140.pdb" /D "NDEBUG" /Gd /Oi /MT /Fa"x64\Release\" /Fo"x64\Release\" /Fp"x64\Release\fixed_point_test.pch"

######################################################################
# add external project, google_test

# helpful example [here](https://gist.github.com/Fraser999/5351180)
ExternalProject_Add(
  google_test
  GIT_REPOSITORY "https://github.com/google/googletest.git"
  GIT_TAG "release-1.7.0"
  INSTALL_COMMAND ""
  CMAKE_ARGS -Dgtest_force_shared_crt=ON
)
ExternalProject_Get_Property(google_test source_dir)
include_directories(SYSTEM ${source_dir}/include)

######################################################################
# fp_test target

add_executable(
  fp_test
  ${CMAKE_CURRENT_LIST_DIR}/fixed_point.cpp
  ${CMAKE_CURRENT_LIST_DIR}/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/proposal.cpp
  ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
)
set_target_properties(
        fp_test
        PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS}"
        OUTPUT_NAME "fp_test"
)

######################################################################
# hitch google_test to fp_test

add_dependencies(fp_test google_test)
ExternalProject_Get_Property(google_test binary_dir)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  target_link_libraries(
    fp_test
    debug ${binary_dir}/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
    optimized ${binary_dir}/Release/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
  )
else ()
  target_link_libraries(
    fp_test
    general ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a
    general pthread
  )
endif()
