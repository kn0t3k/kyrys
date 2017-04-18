include_directories(include)
set(MBEDTLS_SOURCE_FILES library/sha512.c)
add_library(mbedTLS ${MBEDTLS_SOURCE_FILES})
