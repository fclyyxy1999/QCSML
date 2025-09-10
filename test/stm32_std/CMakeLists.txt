cmake_minimum_required(VERSION 3.22)

# project settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Core project settings
project(DemoForSTD C ASM)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

add_definitions(-DUSE_STDPERIPH_DRIVER -DSTM32F40_41xxx)

file(GLOB_RECURSE SOURCES
        "CMSIS/*.*"
        "STARTUP/*.*"
        "User/*.*"
        "Lib/src/*.*")

# Create an executable object type
add_executable(${PROJECT_NAME}
        ${SOURCES}
        ../benchmarks/benchmark.c
        startup_stm32f407xx.s
)

target_include_directories(${PROJECT_NAME} PRIVATE
        CMSIS
        Lib/inc
        User/inc
        ../benchmarks
)

# Add linked libraries
target_link_libraries(${PROJECT_NAME} QCSML)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
