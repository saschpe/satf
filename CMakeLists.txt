cmake_minimum_required(VERSION 2.6)
project(satf)

set(CMAKE_BUILD_TYPE Release)

find_package(Boost 1.38 COMPONENTS date_time filesystem program_options thread REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(satf src/main.cpp)
target_link_libraries(satf ${Boost_LIBRARIES})
