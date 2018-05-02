add_library(cnl_dsp INTERFACE)

set(cnl_dsp_public_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/biquad.h
        ${CMAKE_CURRENT_SOURCE_DIR}/biquad_cascade.h
        ${CMAKE_CURRENT_SOURCE_DIR}/circular_buffer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/complex.h
        ${CMAKE_CURRENT_SOURCE_DIR}/complex_vector.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dsp_math.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dsp_types.h
        ${CMAKE_CURRENT_SOURCE_DIR}/trig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/virtual_float.h
        ${CMAKE_CURRENT_SOURCE_DIR}/fft.h
        ${CMAKE_CURRENT_SOURCE_DIR}/stft.h
        )

target_sources(cnl_dsp INTERFACE ${cnl_dsp_public_headers})

target_include_directories(cnl_dsp INTERFACE ${CMAKE_CURRENT_LIST_DIR})

install(
        DIRECTORY .
        DESTINATION include/dsp
)