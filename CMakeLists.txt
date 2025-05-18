cmake_minimum_required(VERSION 3.29)
project(git)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
include_directories(.)

add_executable(git
    agent.cpp
    agent.h
    array.hpp
    csvgraph.cpp
    csvgraph.h
    csvparser.cpp
    csvparser.h
    graph.cpp
    graph.h
    gtest_lite.h
    log.hpp
    main.cpp
    memtrace.cpp
    memtrace.h
    mytime.cpp
    mytime.h
    pathfinder.cpp
    pathfinder.h
    sorted_list.hpp
    test.cpp)
