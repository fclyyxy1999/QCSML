cmake_minimum_required(VERSION 3.22)

# project settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Core project settings
project(STM32 C ASM)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

if(USE_ASM)
    set(TARGET_NAME "STM32_STD_ASM")
else()
    set(TARGET_NAME "STM32_STD_NO_ASM")
endif()

add_definitions(-DUSE_STDPERIPH_DRIVER -DSTM32F40_41xxx)

file(GLOB_RECURSE SOURCES
        "CMSIS/*.*"
        "STARTUP/*.*"
        "User/*.*"
        "Lib/src/*.*")

# Create an executable object type
add_executable(${TARGET_NAME} ${SOURCES})

target_include_directories(${TARGET_NAME} PRIVATE
        CMSIS
        Lib/inc
        User/inc)

# Add linked libraries
target_link_libraries(${TARGET_NAME} QCSML)

#set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
#set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
#
#add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
#        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
#        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
#        COMMENT "Building ${HEX_FILE}
#Building ${BIN_FILE}")
