include_directories(include)
file(GLOB MBEDTLS_SOURCE_FILES src/*.c)
add_library(mbedTLS ${MBEDTLS_SOURCE_FILES})
