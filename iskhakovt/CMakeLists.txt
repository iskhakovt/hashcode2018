cmake_minimum_required(VERSION 3.9)
project(iskhakovt)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++")

add_executable(iskhakovt main.cpp types.cpp greedy_basic_iskhakovt.cpp)