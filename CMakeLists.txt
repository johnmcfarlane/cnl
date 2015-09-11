cmake_minimum_required(VERSION 2.8.4)
project(fixed_point_test)

set(WARNING_FLAGS "-Wall -Wextra -Wfatal-errors -Werror")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -fno-rtti -Ofast")
set(CMAKE_EXE_LINKER_FLAGS "-g")

set(SG14_SOURCE_DIRECTORY "include")
set(SG14_TEST_SOURCE_DIRECTORY "src")

set(SOURCE_FILES
    ${SG14_TEST_SOURCE_DIRECTORY}/main.cpp
    ${SG14_TEST_SOURCE_DIRECTORY}/fixed_point_test.cpp)

add_executable(fixed_point_test ${SOURCE_FILES})

include_directories("${SG14_SOURCE_DIRECTORY}")

target_link_libraries(fixed_point_test)
