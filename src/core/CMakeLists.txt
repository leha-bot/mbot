set(CORE_SOURCES
    main.cpp
    bot.cpp
    ../../include/core/bot.hpp
    ../../include/core/module_manager.hpp
    )
add_executable(mbot ${CORE_SOURCES})
target_link_libraries(mbot mbot-utils ${Boost_LIBRARIES})
