if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif()
if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

project(cnl)

cmake_minimum_required(VERSION 3.10.2)

include(CTest)
add_subdirectory("test")

# the CNL library
add_library(Cnl INTERFACE)

target_include_directories(
        Cnl INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

install(TARGETS Cnl EXPORT CnlTargets)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT CnlTargets
        FILE CnlConfig.cmake
        NAMESPACE Cnl::
        DESTINATION lib/cmake/cnl)
