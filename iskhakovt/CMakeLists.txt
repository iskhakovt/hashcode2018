cmake_minimum_required(VERSION 3.0)
project(iskhakovt)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -O3")

find_package(Threads)

add_executable(iskhakovt main.cpp types.cpp greedy_keep_many_iskhakovt.cpp)

target_link_libraries(iskhakovt ${CMAKE_THREAD_LIBS_INIT})
