cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Core project settings
project(DemoForHAl C ASM)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Create an executable object type
add_executable(${PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${PROJECT_NAME} PRIVATE
    # Add user sources here
        ../benchmarks/benchmark.c
)

# Add include paths
target_include_directories(${PROJECT_NAME} PRIVATE
    # Add user defined include paths
        ../benchmarks/
)

# Add project symbols (macros)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${PROJECT_NAME}
    stm32cubemx
    # Add user defined libraries
        QCSML
)




