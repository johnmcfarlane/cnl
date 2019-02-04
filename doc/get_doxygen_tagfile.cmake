
# Generate list of files that are passed to Doxyfile as the INPUT parameter.
file(GLOB_RECURSE doc_INPUTFILES "${CMAKE_CURRENT_LIST_DIR}/../include/*")
list(APPEND doc_INPUTFILES "${CMAKE_CURRENT_LIST_DIR}/index.md")
string (REPLACE ";" " " doc_INPUTFILES "${doc_INPUTFILES}")

# Try to download a tag file for the standard library from a given source.
# If successful, populate doc_TAGFILE accordingly (as used in Doxyfile.in).
macro(GET_DOXYGEN_TAGFILE FILENAME DOWNLOAD_LOC TAGFILE_LOC)
    if (NOT doc_TAGFILE)
        set(DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}")

        # if a std library reference isn't present already
        if (EXISTS ${DESTINATION})
            set(doc_TAGFILE "${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}=${TAGFILE_LOC}")
        else ()
            # try to download it
            set(DOWNLOAD_URL "${DOWNLOAD_LOC}/${FILENAME}")
            file(DOWNLOAD
                    "${DOWNLOAD_URL}"
                    "${DESTINATION}"
                    STATUS doc_TAGFILE_STATUS)

            list(GET doc_TAGFILE_STATUS 0 doc_TAGFILE_STATUS_CODE)
            if (doc_TAGFILE_STATUS EQUAL 0)
                set(doc_TAGFILE "${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}=${TAGFILE_LOC}")
            else ()
                list(GET doc_TAGFILE_STATUS 1 doc_TAGFILE_STATUS_MESSAGE)
                message("Failed to download Doxfile for standard library from \"${DOWNLOAD_URL}\": ${doc_TAGFILE_STATUS_MESSAGE}")
            endif ()
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

configure_file("${CMAKE_CURRENT_LIST_DIR}/Doxyfile.in" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
