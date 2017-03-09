cmake_minimum_required(VERSION 3.6)
#include(cmake/version_detect.cmake)
project(mbot ${VERSION})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
find_package(Boost REQUIRED COMPONENTS system thread filesystem date_time)
add_subdirectory(external/tgbot-cpp)

include_directories(${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/external/tgbot-cpp/include)

add_subdirectory(src)
