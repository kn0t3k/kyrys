cmake_minimum+required(VERSION 3.6)

set(PROJECT_NAME Kyrys)

project(PROJECT_NAME)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS -std=c++11 -g -pedantic -Wall -Wextra -Werror")

set(SOURCE_FILES src/main.cpp)

add_executable(PROJECT_NAME ${SOURCE_FILES})
