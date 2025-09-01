cmake_minimum_required(VERSION 3.22)
project(QCSML LANGUAGES C ASM)

option(ARCH_ARMV7 "Build for ARMv7" OFF)
option(ARCH_X86 "Build for x86" OFF)
option(USE_ASM "Use assembly optimization" OFF)
option(BUILD_TESTS "Build test programs" OFF)
option(BUILD_EMBEDDED "Build embedded target" OFF)

# arch detection
if(ARCH_ARMV7)
    message(STATUS "Building for ARMv7 architecture")
    add_definitions(-DARCH_ARMV7)
    set(ARCH_NAME "armv7")
elseif(ARCH_X86)
    message(STATUS "Building for x86 architecture")
    add_definitions(-DARCH_X86)
    set(ARCH_NAME "x86")
else()
    message(FATAL_ERROR "Unsupported architecture")
endif()

include_directories(${CMAKE_SOURCE_DIR}/inc)

set(COMMON_SOURCES
        src/asn1.c
        src/rand/rand.c)

set(SM2_PUBLIC
    src/sm2/core_public.c
    src/sm2/sm2_table.c
    src/sm2/sm2_key.c
    src/sm2/sm2_sign.c
    src/sm2/sm2_enc.c)

set(SM3_PUBLIC
    src/sm3/sm3_public.c)

set(SM4_PUBLIC
    src/sm4/sm4_public.c
    src/sm4/sm4_tables.c)

# enable assembly or pure C implementation
if(USE_ASM)
    message(STATUS "Using assembly implementation")
    set(IMPL_SUFFIX "_asm")
    set(SM2_SOURCES
        ${SM2_PUBLIC}
        src/sm2/asm/sm2_fn_armv7.S
        src/sm2/asm/sm2_fp_armv7.S
    )
    set(SM3_SOURCES
        ${SM3_PUBLIC}
        src/sm3/asm/sm3_core_armv7.S
    )
    set(SM4_SOURCES
        ${SM4_PUBLIC}
        src/sm4/asm/sm4_core_armv7.S
    )
else()
    message(STATUS "Using pure C implementation")
    set(IMPL_SUFFIX "_no_asm")
    set(SM2_SOURCES
        ${SM2_PUBLIC}
        src/sm2/noasm_core.c
    )
    set(SM3_SOURCES
        ${SM3_PUBLIC}
        src/sm3/noasm_sm3.c
    )
    set(SM4_SOURCES
        ${SM4_PUBLIC}
        src/sm4/noasm_sm4.c
    )
endif()

# Create QCSML static library
add_library(QCSML STATIC
        ${SM2_SOURCES}
        ${SM3_SOURCES}
        ${SM4_SOURCES}
        ${COMMON_SOURCES})

set_target_properties(QCSML PROPERTIES
        OUTPUT_NAME "QCSML_${ARCH_NAME}${IMPL_SUFFIX}_${CMAKE_BUILD_TYPE}"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Add executable for testing (Only building on x86 or non-embedded systems)
if(BUILD_TESTS)
    # main test
    add_executable(sm_test test/testcode/test.c)

    target_link_libraries(sm_test QCSML)
    set_target_properties(sm_test PROPERTIES
            OUTPUT_NAME "sm_test_${ARCH_NAME}${IMPL_SUFFIX}"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

    # benchmark test
    add_executable(benchmark_test test/speed/speed.c)
    target_link_libraries(benchmark_test QCSML)
    set_target_properties(benchmark_test PROPERTIES
            OUTPUT_NAME "benchmark_test_${ARCH_NAME}${IMPL_SUFFIX}"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()

if(BUILD_EMBEDDED AND ARCH_ARMV7)
#    add_subdirectory(test/STM32_LL)
    add_subdirectory(test/STM32_STD)
endif()