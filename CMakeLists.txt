cmake_minimum_required(VERSION 3.6)
project(mbot)

set(CMAKE_CXX_STANDARD 11)
find_package(Boost REQUIRED COMPONENTS system thread filesystem date_time)
include_directories(${Boost_INCLUDE_DIRS})

include(cmake/version_detect.cmake)
add_subdirectory(src)
