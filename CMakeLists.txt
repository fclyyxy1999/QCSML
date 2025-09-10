cmake_minimum_required(VERSION 3.28.3)
project(QCSML C ASM)
set(CMAKE_C_STANDARD 11)

set(ARCH_OPTIONS "x86" "armv7")
set(ARCH "x86" CACHE STRING "Target architecture")
set_property(CACHE ARCH PROPERTY STRINGS ${ARCH_OPTIONS})
option(ASM "Enable Assembly Optimizations" OFF)

include_directories(
        inc
)

if(ASM AND ARCH STREQUAL "armv7")
    set(    ASM_SOURCE
            src/sm2/asm/sm2_core_armv7.s
            src/sm3/asm/sm3_core_armv7.s
            src/sm4/asm/sm4_core_armv7.s
    )
endif()

set(    COMMON_SOURCE
        src/common/common.c
        src/asn1/asn1.c
        src/rand/rand.c)

set(    SM2_SOURCE
        src/sm2/sm2_core.c
        src/sm2/sm2_table.c
        src/sm2/sm2_key.c
        src/sm2/sm2_sign.c
        src/sm2/sm2_enc.c)

set(    SM3_SOURCE
        src/sm3/sm3.c)

set(    SM4_SOURCE
        src/sm4/sm4.c)

# Create QCSML static library
add_library(QCSML STATIC
        ${ASM_SOURCE}
        ${COMMON_SOURCE}
        ${SM2_SOURCE}
        ${SM3_SOURCE}
        ${SM4_SOURCE})
target_include_directories(QCSML PUBLIC inc)
set_target_properties(QCSML PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if(ARCH STREQUAL "x86")
    message(STATUS "Building for x86 architecture")
    add_executable(
            benchmarks
            test/benchmarks/benchmark.c
            test/benchmarks/benchmark.h
            test/benchmarks/main.c)
    target_link_libraries(benchmarks PRIVATE QCSML)
    set_target_properties(benchmarks PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
elseif (ARCH STREQUAL "armv7")
    message(STATUS "Building for armv7 architecture")
    add_subdirectory(test/stm32_hal)
    add_subdirectory(test/stm32_std)
endif ()