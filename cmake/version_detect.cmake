option(MBOT_VERSION "Skip version detection using Git tags and set version." 0.0.0)

if(MBOT_VERSION EQUAL 0.0.0)
    message("Skipping version detection using Git tags...")
    add_definitions(-DMBOT_VERSION="${MBOT_VERSION}")
    set(VERSION ${MBOT_VERSION})
else()
    find_package(Git QUIET)
    if(GIT_EXECUTABLE)
        execute_process(
                COMMAND           "${GIT_EXECUTABLE} status"
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                RESULT_VARIABLE   git_status_result
                OUTPUT_VARIABLE   git_status_output
                ERROR_VARIABLE    git_status_error
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_STRIP_TRAILING_WHITESPACE
        )
        if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.git)
            include(${CMAKE_SOURCE_DIR}/cmake/version_from_git.cmake)
            version_from_git(GIT_EXECUTABLE ${GIT_EXECUTABLE}
                             INCLUDE_HASH ON
                             )
            add_definitions(-DMBOT_VERSION="${SEMVER}")
        else()
            message(WARNING "Setting version to null string, because building outside of Git repository, please specify version by setting MBOT_VERSION CMake variable.")
            add_definitions(-DMBOT_VERSION="unknown")
            set(VERSION 0.0.0)
        endif()
    else()
        message(WARNING "Setting version to null string, because Git executable not found, please specify version by setting MBOT_VERSION CMake variable.")
        add_definitions(-DMBOT_VERSION="unknown")
        set(VERSION 0.0.0)
    endif()
endif()
