include_directories(include)
set(MBEDTLS_SOURCE_FILES library/sha512.c library/aes.c library/padlock.c library/aesni.c)
add_library(mbedTLS ${MBEDTLS_SOURCE_FILES})
