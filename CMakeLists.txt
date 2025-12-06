cmake_minimum_required(VERSION 3.10)
project(BoardGames)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Include common headers directory
include_directories(include)

# Recursively collect all .cpp files in src and subfolders
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define executable with all source files
add_executable(BoardGames ${SOURCES})

# (Optional) You can add warnings or other compile options, e.g.:
# target_compile_options(BoardGames PRIVATE /W4)  # MSVC warning level 4
# target_compile_options(BoardGames PRIVATE -Wall -Wextra)  # GCC/Clang

# Copy dic.txt from src/Word_Tic_Tac_Toe to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/word/dic.txt DESTINATION ${CMAKE_BINARY_DIR})
