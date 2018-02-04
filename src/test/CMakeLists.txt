include("../common/common.cmake")

######################################################################
# fp_test target

add_executable(fp_test
        bit.cpp
        cnlint.cpp
        constant.cpp
        encompasses.cpp
        boost.simd.cpp
        common.cpp
        papers/p0381.cpp
        papers/p0675.cpp
        papers/p0827.cpp
        limits.cpp
        num_traits.cpp
        numeric.cpp
        number_test.cpp
        fixed_point/p0037.cpp
        readme.cpp
        overflow/overflow.cpp
        cppnow2017.cpp
        glm.cpp
        index.cpp
        zero_cost_square.cpp
        boost.multiprecision.cpp
        safft.cpp
        fixed_point/fixed_point_built_in.cpp
        fixed_point/constants.cpp
        precise/elastic/rounding_elastic_integer.cpp
        elastic/elastic_integer.cpp
        fixed_point/rounding_fixed_point.cpp
        fixed_point/extras.cpp
        papers/p0554.cpp
        elastic/precise/overflow/precise_safe_integer.cpp
        zero_cost_free_functions.cpp
        fixed_point/elastic/precise/overflow/rounding_safe_fixed_point.cpp
        snippets.cpp
        overflow/elastic/safe_integer.cpp
        overflow/overflow_integer.cpp
        fixed_point/elastic/make_elastic_fixed_point.cpp
        fixed_point/math.cpp
        zero_cost_average.cpp
        cppcon2017.cpp
        precise/rounding_integer.cpp
        fixed_point/elastic/elastic_fixed_point.cpp
        fixed_point/overflow/fixed_point_native_integer.cpp
        fixed_point/overflow/fixed_point_throwing_integer.cpp
        fixed_point/overflow/fixed_point_saturated_integer.cpp
        main.cpp)

######################################################################
# find external lib, boost.multiprecision

find_package(Boost)

if(Boost_FOUND)
    if(Boost_VERSION GREATER 105500 OR EXCEPTIONS)
        # only compile multiprecision.cpp if Boost.Multiprecision is available
        set(BOOST_FOUND_CXX_FLAGS "-DCNL_BOOST_ENABLED")
        target_include_directories(fp_test SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
    else(Boost_VERSION GREATER 105500 OR EXCEPTIONS)
        message("Boost version is too low to use without exception handling")
    endif(Boost_VERSION GREATER 105500 OR EXCEPTIONS)
else(Boost_FOUND)
    message("Boost not found")
endif(Boost_FOUND)

######################################################################
# pull external project, boost.simd

if(Boost_FOUND AND Boost_VERSION GREATER 106099)
    # Boost.SIMD has problems building with gcc-7.x
    if(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.0.0")
        ExternalProject_Add(
                boost.simd
                URL "https://github.com/numscale/boost.simd/archive/b1e046e3ffa720226e45512c76c505f34cde7be2.zip"
                URL_MD5 "74fc0bc279a18f931e4d2ec026af8dd4"
                UPDATE_COMMAND ""
                INSTALL_COMMAND ""
        )

        ExternalProject_Get_Property(boost.simd source_dir)
        target_include_directories(
                fp_test
                PRIVATE
                SYSTEM
                "${source_dir}/include"
        )

        add_dependencies(
                fp_test
                boost.simd
        )

        set(BOOST_SIMD_FOUND_CXX_FLAGS "-DCNL_BOOST_SIMD_ENABLED")
    endif(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU OR CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.0.0")
endif(Boost_FOUND AND Boost_VERSION GREATER 106099)

######################################################################
# pull external project, glm

ExternalProject_Add(
    glm
    URL "https://github.com/johnmcfarlane/glm/archive/878086621522255b14017c26d1df3b9fb0b36009.zip"
    URL_MD5 "fe3b032c230df63474bc26549526aaec"
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(glm source_dir)
target_include_directories(
    fp_test
    PRIVATE
    SYSTEM
    ${source_dir}
)

add_dependencies(
    fp_test
    glm
)

######################################################################
# pull external project, google_test

ExternalProject_Add(
        google_test
        URL "https://github.com/google/googletest/archive/d6a4478caf10f5cfcfe477e576f4a290d9a49b24.zip"
        URL_MD5 "28aedb06b73ed59ca59c1c973a197cc9"
        UPDATE_COMMAND ""
        INSTALL_COMMAND ""
        CMAKE_ARGS -Dgtest_force_shared_crt=ON
)

ExternalProject_Get_Property(google_test source_dir)
target_include_directories(
        fp_test
        PRIVATE
        SYSTEM
        ${source_dir}/googletest/include
)

ExternalProject_Get_Property(google_test binary_dir)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    target_link_libraries(
            fp_test
            debug ${binary_dir}/googlemock/gtest/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}gtestd${CMAKE_FIND_LIBRARY_SUFFIXES}
            optimized ${binary_dir}/googlemock/gtest/Release/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
    )
else ()
    target_link_libraries(
            fp_test
            general "${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a"
            general pthread
    )
endif ()

add_dependencies(
        fp_test
        google_test
)

######################################################################
# fp_test compiler flags

set_target_properties(
        fp_test
        PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS} ${BOOST_FOUND_CXX_FLAGS} ${BOOST_SIMD_FOUND_CXX_FLAGS}"
)

target_link_libraries(fp_test cnl)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set_source_files_properties(
            fixed_point/overflow/fixed_point_saturated_integer.cpp
            PROPERTIES COMPILE_FLAGS "/wd4018")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set_source_files_properties(
            boost.multiprecision.cpp
            index.cpp
            PROPERTIES COMPILE_FLAGS "-fpermissive")

    set_source_files_properties(
            cppcon2017.cpp
            fixed_point/overflow/fixed_point_saturated_integer.cpp
            PROPERTIES COMPILE_FLAGS "-Wno-sign-compare -Wno-strict-overflow")

    set_source_files_properties(
            overflow/overflow_integer.cpp
            PROPERTIES COMPILE_FLAGS "-Wno-sign-compare")

    set_source_files_properties(
            fixed_point/math.cpp
            PROPERTIES COMPILE_FLAGS "-Wno-integer-overflow")

    set_source_files_properties(
            fixed_point/fixed_point_built_in.cpp
            PROPERTIES COMPILE_FLAGS "-Wno-overflow")
endif ()

######################################################################
# testing

enable_testing()
add_test(all_tests fp_test)
