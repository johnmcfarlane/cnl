# performs conan installation so user doesn't have to issue `conan install`
# command every time they configure cmake

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(REQUIRES
        gtest/1.8.1@bincrafters/stable
        benchmark/1.5.0@johnmcfarlane/stable
        BASIC_SETUP
        BUILD outdated)

include(${CMAKE_BINARY_DIR}/test/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)
