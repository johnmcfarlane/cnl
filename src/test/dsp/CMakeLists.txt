include_directories(
        ${PROJECT_SOURCE_DIR}/include
)
message("INT128_FLAGS are: ${INT128_FLAGS}")

set(sample_dsp_sources
        basic_math.cpp
        biquad.cpp
        biquad_cascade.cpp
        trig.cpp
        complex.cpp
        virtual_float.cpp
        complex_vector.cpp
        fft.cpp
        stft.cpp
        )

foreach(source ${sample_dsp_sources})
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
        make_test("${source}" target "-fpermissive -Wno-sign-compare")
    else ()
        make_test("${source}" target "")
    endif ()
endforeach(source)