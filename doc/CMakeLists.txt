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

# Try to download a tag file for the standard library from a given source.
# If successful, populate doc_TAGFILE accordingly (as used in Doxyfile.in).
macro(GET_DOXYGEN_TAGFILE FILENAME DOWNLOAD_LOC TAGFILE_LOC)
    if (NOT doc_TAGFILE)
        set(DOWNLOAD_URL "${DOWNLOAD_LOC}/${FILENAME}")
        # try to download a std library reference from cppreference
        file(DOWNLOAD
                "${DOWNLOAD_URL}"
                "${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}"
                STATUS doc_TAGFILE_STATUS)

        list(GET doc_TAGFILE_STATUS 0 doc_TAGFILE_STATUS_CODE)
        if (doc_TAGFILE_STATUS EQUAL 0)
            set(doc_TAGFILE "${cnl_BINARY_DIR}/doc/${FILENAME}=${TAGFILE_LOC}")
        else ()
            list(GET doc_TAGFILE_STATUS 1 doc_TAGFILE_STATUS_MESSAGE)
            message("Failed to download Doxfile for standard library from \"${DOWNLOAD_URL}\": ${doc_TAGFILE_STATUS_MESSAGE}")
        endif ()
    endif ()
endmacro(GET_DOXYGEN_TAGFILE)

# try to download a tag file for the standard library from cppreference
get_doxygen_tagfile(
        "cppreference-doxygen-web.tag.xml"
        "http://upload.cppreference.com/mwiki/images/f/f8"
        "http://en.cppreference.com/w/")

# try to download a tag file for the standard library from libstdc++
get_doxygen_tagfile(
        "libstdc++.tag"
        "https://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen"
        "https://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/")

configure_file(Doxyfile.in Doxyfile @ONLY)
add_custom_target(Doc ALL
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)

