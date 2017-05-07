set(TEST_NAME kyrys_test)

set(CATCH_DIR ../lib/catch)
add_library(Catch INTERFACE)

include_directories(Catch INTERFACE ${CATCH_DIR})
include_directories("../include")

file(GLOB TEST_SOURCES *.cpp)

add_executable(${TEST_NAME} ${TEST_SOURCES})
target_include_directories(Catch INTERFACE ${CATCH_DIR})
target_link_libraries(${TEST_NAME} Catch src_library mbedTLS)

qt5_use_modules(${TEST_NAME} ${QT_USED_MODULES})
