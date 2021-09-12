cmake_minimum_required(VERSION 3.20)
project(Listener)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(Listener main.cpp utils.cpp utils.h sender.cpp sender.h receiver.cpp receiver.h)
