cmake_minimum_required(VERSION 3.6)
#include(cmake/version_detect.cmake)
project(mbot ${VERSION})

set(CMAKE_CXX_STANDARD 11)
find_package(Boost REQUIRED COMPONENTS system thread filesystem date_time)
include_directories(${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/external/tgbot/include)

add_subdirectory(external/tgbot-cpp)
add_subdirectory(src)
