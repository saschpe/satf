cmake_minimum_required(VERSION 2.6)
project(satf)

find_package(Boost 1.38 COMPONENTS date_time filesystem program_options thread REQUIRED)

set(satf_SRCS
    src/utility.cpp
    src/main.cpp
)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(satf ${satf_SRCS})
target_link_libraries(satf ${Boost_LIBRARIES})
