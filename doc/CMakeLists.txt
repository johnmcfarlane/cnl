# add a target to generate API documentation with Doxygen
find_package(Doxygen)

if (NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen not found.")
    return()
endif (NOT DOXYGEN_FOUND)

# Generate list of files that are passed to Doxyfile as the INPUT parameter.
file(GLOB_RECURSE doc_INPUTFILES "${CMAKE_HOME_DIRECTORY}/include/*")
list(APPEND doc_INPUTFILES "${CMAKE_CURRENT_LIST_DIR}/index.md")
string (REPLACE ";" " " doc_INPUTFILES "${doc_INPUTFILES}")

add_custom_target(Doc ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)

add_custom_target(TagFiles
        COMMAND ${CMAKE_COMMAND} -P /home/john/ws/most_negative/cnl/doc/get_doxygen_tagfile.cmake)
add_dependencies(Doc TagFiles)
