cmake_minimum_required(VERSION 3.27)
project(DiningPhilosophers)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(SO2 main.cpp
        philosophers.cpp
        philosophers.h
)