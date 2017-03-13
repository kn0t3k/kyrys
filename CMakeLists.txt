cmake_minimum_required (VERSION 2.8)

set(PROJECT_NAME Kyrys)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Wextra -pedantic")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(DIRS lib/mbedTLS)
subdirs(${DIRS})
include_directories(${DIRS}/include)
include_directories("include")

file(GLOB SOURCES src/*.cpp)
file(GLOB TEST_SOURCES test/*.cpp)

set(CATCH_DIR lib/catch)
set(TEST_NAME kyrys_test)

add_library(Catch INTERFACE)
add_library(src_library STATIC ${SOURCES})

#tests
add_executable(${TEST_NAME} ${TEST_SOURCES})
target_include_directories(Catch INTERFACE ${CATCH_DIR})
target_link_libraries(${TEST_NAME} Catch mbedTLS src_library)

enable_testing()
add_test(NAME ${TEST_NAME} COMMAND test)

#executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} mbedTLS src_library)
