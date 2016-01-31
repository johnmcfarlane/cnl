cmake_minimum_required(VERSION 2.8.11)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_custom_target(doc ALL
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif (DOXYGEN_FOUND)
