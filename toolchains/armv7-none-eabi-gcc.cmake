set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

# Some default GCC settings
# arm-none-eabi- must be part of path environment
set(TOOLCHAIN_PREFIX                arm-none-eabi-)

set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# arch flags
set(ARCH_ARMV7 ON CACHE BOOL "ARMv7 architecture")
set(SUPPORT_ASM ON CACHE BOOL "Enable assembly for ARMv7")

# STM32F407 Cortex-M4 Build Flags (Well-Organized & English Comments)

# ----------------------------------------------------------------------------
# 1. Architecture Flags
# ----------------------------------------------------------------------------
set(ARCH_FLAGS
        -mcpu=cortex-m4           # CPU type
        -mthumb                   # Use Thumb instruction set
        -mthumb-interwork         # Support Thumb/ARM interworking
)

# ----------------------------------------------------------------------------
# 2. FPU/Float Flags
# ----------------------------------------------------------------------------
set(FPU_FLAGS
        -mfpu=fpv4-sp-d16         # FPU type
        -mfloat-abi=hard          # Use hardware floating point
        -u_printf_float           # Enable float support for printf
)

# ----------------------------------------------------------------------------
# 3. Optimization Flags
# ----------------------------------------------------------------------------
set(OPT_FLAGS
#        -Ofast                    # Maximum optimization
        -funroll-loops            # Unroll loops
        -fprefetch-loop-arrays    # Prefetch loop arrays
        -fomit-frame-pointer      # Omit frame pointer
        -ffunction-sections       # Place each function in its own section
        -fdata-sections           # Place each data item in its own section
        -flto
)

# ----------------------------------------------------------------------------
# 4. Code Behavior/Compatibility Flags
# ----------------------------------------------------------------------------
set(CODE_FLAGS
        -fno-exceptions           # Disable C++ exceptions
        -fno-common               # Disable common global variables
        -fmessage-length=0        # Set message length to 0
        -mapcs-frame              # Use APCS frame format
        -mapcs-stack-check        # Enable APCS stack checking
)

# ----------------------------------------------------------------------------
# 5. Assembler Flags
# ----------------------------------------------------------------------------
set(ASM_FLAGS
        -x assembler-with-cpp     # Use C preprocessor for assembler
        -MMD                      # Generate dependency files
        -MP                       # Add phony targets for dependencies
)

# ----------------------------------------------------------------------------
# 6. Linker Flags
# ----------------------------------------------------------------------------
set(LINK_SCRIPT
        -T${CMAKE_SOURCE_DIR}/test/STM32_LL/STM32F407XX_FLASH.ld # Linker script
)

set(LINK_FLAGS
        -Wl,-gc-sections              # Remove unused sections
        -Wl,--print-memory-usage      # Print memory usage
        -Wl,-Map=stm32.map            # Generate map file
        --specs=nano.specs
        ${LINK_SCRIPT}
        ${ARCH_FLAGS}
        ${FPU_FLAGS}
        -flto
        -mthumb
        -mthumb-interwork
)

# ----------------------------------------------------------------------------
# 7. Math Library Definitions
# ----------------------------------------------------------------------------
add_compile_definitions(
        ARM_MATH_CM4
        ARM_MATH_MATRIX_CHECK
        ARM_MATH_ROUNDING
)

# ----------------------------------------------------------------------------
# 8. Set C/ASM/Link Flags
# ----------------------------------------------------------------------------
string(JOIN " " C_FLAGS_STR ${ARCH_FLAGS} ${FPU_FLAGS} ${OPT_FLAGS} ${CODE_FLAGS})
string(JOIN " " ASM_FLAGS_STR ${ARCH_FLAGS} ${FPU_FLAGS} ${OPT_FLAGS} ${CODE_FLAGS} ${ASM_FLAGS})
string(JOIN " " LINK_FLAGS_STR ${LINK_FLAGS})

set(CMAKE_C_FLAGS "${C_FLAGS_STR}")
set(CMAKE_ASM_FLAGS "${ASM_FLAGS_STR}")
set(CMAKE_C_LINK_FLAGS "${LINK_FLAGS_STR}")

# ----------------------------------------------------------------------------
# 9. Print Final Flags (for debugging)
# ----------------------------------------------------------------------------
message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_ASM_FLAGS: ${CMAKE_ASM_FLAGS}")
message(STATUS "CMAKE_C_LINK_FLAGS: ${CMAKE_C_LINK_FLAGS}")