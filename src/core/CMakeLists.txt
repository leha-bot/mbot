set(CORE_SOURCES
    main.cpp
    bot.cpp
    ../../include/core/bot.hpp
    ../../include/core/module_manager.hpp
    command_manager.cpp
    ../../include/core/command_manager.hpp
    )
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)
add_executable(mbot ${CORE_SOURCES})
target_link_libraries(mbot mbot-utils TgBot ${Boost_LIBRARIES})
