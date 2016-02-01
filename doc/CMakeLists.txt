cmake_minimum_required(VERSION 2.8.11)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_custom_target(doc ALL
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif (DOXYGEN_FOUND)

# run cmake (if installed) on papers
find_program(CMARK_EXECUTABLE cmark)
if (CMARK_EXECUTABLE)
    foreach(PAPER p0037 p0037r0)
        message("${CMAKE_CURRENT_LIST_DIR}/${PAPER}.md -> ${CMAKE_CURRENT_LIST_DIR}/gh-pages/${PAPER}.html")
        execute_process(
                COMMAND ${CMARK_EXECUTABLE}
                INPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/${PAPER}.md
                OUTPUT_FILE ${CMAKE_CURRENT_LIST_DIR}/gh-pages/${PAPER}.html
        )
    endforeach(PAPER)
endif (CMARK_EXECUTABLE)
