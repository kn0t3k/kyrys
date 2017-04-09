cmake_minimum_required (VERSION 3.0)
cmake_policy(SET CMP0020 OLD)

set(QT_INCLUDE_DIR "C:/Qt/Qt5.8.0/5.8/mingw53_32/include")
set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.8.0/5.8/mingw53_32/lib/cmake")

set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

set(PROJECT_NAME Kyrys)
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++14 -g -Wall -Wextra -pedantic --coverage")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(MBEDTLS_DIR lib/mbedTLS)
subdirs(${MBEDTLS_DIR})
include_directories(${MBEDTLS_DIR}/include)
include_directories("include")
include_directories(${QT_INCLUDE_DIR})

file(GLOB SOURCES src/*.cpp include/*.hpp)

add_library(src_library STATIC ${SOURCES})

add_definitions(-DTEMP_DIR="C:/__TEMP__")

enable_testing()
add_subdirectory(test)

#executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} src_library)
qt5_use_modules(${PROJECT_NAME} Network)


################################################
#gcov: C:\skola\kyrys\kyrys\gcov>gcov ../test/*.cpp --object-directory C:\skola\kyrys\kyrys\test\CMakeFiles
#		\kyrys_test.dir
