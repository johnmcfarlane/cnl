cmake_minimum_required(VERSION 2.8.11)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_custom_target(doc ALL
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif (DOXYGEN_FOUND)

# add a target to generate HTML files from commonmark files
find_program(CMARK_EXECUTABLE cmark)
if (CMARK_EXECUTABLE)
    foreach(PAPER p0037 p0037r0)
        set(HTML_FILE ${CMAKE_CURRENT_LIST_DIR}/gh-pages/${PAPER}.html)
        set(COMMONMARK_FILE ${CMAKE_CURRENT_LIST_DIR}/${PAPER}.md)
        add_custom_target(
                "commonmark_${PAPER}" ALL
                COMMAND ${CMARK_EXECUTABLE} ${COMMONMARK_FILE} > ${HTML_FILE}
                DEPENDS ${COMMONMARK_FILE}
        )
    endforeach(PAPER)
endif (CMARK_EXECUTABLE)
