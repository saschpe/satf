cmake_minimum_required(VERSION 2.6)
project(sbslab)

set(CMAKE_MODULE_PATH "${sbslab_SOURCE_DIR}/cmake/modules")

find_package(Boost 1.38 COMPONENTS thread REQUIRED)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(satf_SRCS
        src/main.cpp
    )
    add_executable(satf ${satf_SRCS})
    target_link_libraries(satf ${Boost_LIBRARIES})
endif()
