set(CORE_SOURCES
    main.cpp
    )

add_executable(mbot ${CORE_SOURCES})
target_link_libraries(mbot mbot-utils)
