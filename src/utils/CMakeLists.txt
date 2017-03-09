set(UTILS_SOURCES
    shared_library.cpp
    ../../include/utils/shared_library.hpp
    logger.cpp
    ../../include/utils/logger.hpp
    log_manager.cpp
    ../../include/utils/log_manager.hpp
    thread_balancer.cpp
    ../../include/utils/thread_balancer.hpp
    )

add_library(mbot-utils STATIC ${UTILS_SOURCES})
target_link_libraries(mbot-utils
                      ${Boost_datetime_LIBRARY}
                      ${CMAKE_DL_LIBS}
                      )
