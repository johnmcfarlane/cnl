project(Tau)

cmake_minimum_required(VERSION 3.2)

add_executable(Tau main.cpp)
target_compile_options(Tau PRIVATE "-std=c++17")

find_package(Cnl REQUIRED)
include(${Cnl_DIR}/CnlConfig.cmake)

target_link_libraries(Tau Cnl::Cnl)
get_target_property(Cnl_INCLUDE_DIR Cnl::Cnl INTERFACE_INCLUDE_DIRECTORIES)

target_include_directories(Tau PRIVATE ${Cnl_INCLUDE_DIR})
