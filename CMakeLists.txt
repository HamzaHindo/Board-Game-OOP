cmake_minimum_required(VERSION 3.10)
project(BoardGames)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include common headers directory
include_directories(include)

# Recursively collect all .cpp files in src and subfolders
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define executable with all source files
add_executable(BoardGames ${SOURCES} "src/main.cpp")

# (Optional) You can add warnings or other compile options, e.g.:
# target_compile_options(BoardGames PRIVATE /W4)  # MSVC warning level 4
# target_compile_options(BoardGames PRIVATE -Wall -Wextra)  # GCC/Clang

