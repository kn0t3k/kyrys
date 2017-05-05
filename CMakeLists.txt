cmake_minimum_required (VERSION 3.0)
cmake_policy(SET CMP0020 OLD)

set(QT_INCLUDE_DIR "C:/Qt/Qt5.8.0/5.8/mingw53_32/include" "F:/Qt/Qt5.8.0/5.8/mingw53_32/include")
set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.8.0/5.8/mingw53_32/lib/cmake" "F:/Qt/Qt5.8.0/5.8/mingw53_32/lib/cmake")

set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

set(SERVER_NAME KyrysServer)
set(CLIENT_NAME KyrysClient)
project(Kyrys)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

set(QT_USED_MODULES Network Sql)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -pedantic")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#Adding mbedTLS library
set(MBEDTLS_DIR lib/mbedTLS)
subdirs(${MBEDTLS_DIR})
include_directories(${MBEDTLS_DIR}/include)

include_directories("include")
include_directories(${QT_INCLUDE_DIR})

file(GLOB SOURCES src/*.cpp include/*.hpp)
list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/src/clientMain.cpp ${CMAKE_SOURCE_DIR}/src/serverMain.cpp)

add_library(src_library STATIC ${SOURCES})

enable_testing()
add_subdirectory(test)

#executable
add_executable(${SERVER_NAME} "src/serverMain.cpp")
add_executable(${CLIENT_NAME} "src/clientMain.cpp")
target_link_libraries(${SERVER_NAME} src_library) 	# not linking mbedTLS
target_link_libraries(${CLIENT_NAME} src_library)	# not linking mbedTLS
target_link_libraries(${SERVER_NAME} src_library mbedTLS)   #added mbdTLS
target_link_libraries(${CLIENT_NAME} src_library mbedTLS)   #added mbdTLS
qt5_use_modules(${SERVER_NAME} ${QT_USED_MODULES})
qt5_use_modules(${CLIENT_NAME} ${QT_USED_MODULES})


################################################
#gcov: C:\skola\kyrys\kyrys\gcov>gcov ../test/*.cpp --object-directory C:\skola\kyrys\kyrys\test\CMakeFiles
#		\kyrys_test.dir
#	set(CMAKE_CXX_FLAGS "-std=c++14 -g -Wall -Wextra -pedantic --coverage")