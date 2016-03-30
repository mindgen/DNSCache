cmake_minimum_required(VERSION 3.4)
project(DNSCache)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pthread")

set(SOURCE_FILES main.cpp src/DNSCacheImpl.cpp src/ThreadLockGuard.cpp)
add_executable(DNSCache ${SOURCE_FILES})