# Enable verbose output
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE)

cmake_minimum_required(VERSION 3.2)
project(game_of_adversary LANGUAGES C ASM)

add_library(core src/core.c)
target_include_directories(core PUBLIC include)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(game_of_adversary core)

target_compile_options(${PROJECT_NAME} PRIVATE -O0 -g)

# Testing
include(CTest)
enable_testing()

add_executable(test_runner src/CTest.c)
target_link_libraries(test_runner core)

target_compile_options(test_runner PRIVATE -O0 -g)

add_test(
    NAME bins_test
    COMMAND test_runner
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
)
