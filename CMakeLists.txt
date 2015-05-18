cmake_minimum_required(VERSION 3.2)
project(SystemArchitecture)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/home/zerophase/ClionProjects/SystemArchitecture")
add_executable(SystemArchitecture ${SOURCE_FILES} FreeListAllocator.cpp FreeListAllocator.h MemoryWrapper.cpp MemoryWrapper.h MemoryConfig.h)