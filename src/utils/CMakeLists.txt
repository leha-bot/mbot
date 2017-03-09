set(UTILS_SOURCES
    shared_library.cpp
    logger.cpp
    log_manager.cpp
    )
add_library(mbot-utils STATIC ${UTILS_SOURCES})
target_link_libraries(mbot-utils
                      ${Boost_datetime_LIBRARY}
                      ${CMAKE_DL_LIBS}
                      )
