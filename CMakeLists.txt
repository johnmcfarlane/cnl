if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif()
if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

project(cnl)

cmake_minimum_required(VERSION 3.5.1)

unset(CNL_STANDALONE)
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(CNL_STANDALONE ON)
endif()

option(CNL_ENABLE_TESTS "Set to true to enable tests" ${CNL_STANDALONE})
if (CNL_ENABLE_TESTS)
    include(CTest)
    add_subdirectory("test")
endif()

# the CNL library
add_library(Cnl INTERFACE)

target_include_directories(
        Cnl INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

option(CNL_ENABLE_INSTALL "Set to true to set install directives" ${CNL_STANDALONE})
if (CNL_ENABLE_INSTALL)
    install(TARGETS Cnl EXPORT CnlTargets)
    install(DIRECTORY include/ DESTINATION include)
    install(EXPORT CnlTargets
            FILE CnlConfig.cmake
            NAMESPACE Cnl::
            DESTINATION lib/cmake/cnl)
endif()
