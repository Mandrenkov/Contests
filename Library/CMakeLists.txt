cmake_minimum_required(VERSION 3.14)

# ------------------------------------------------------------------------------
# Data Structures and Algorithms Library (DSAL)
# ------------------------------------------------------------------------------

message("
8888888b.   .d8888b.        d8888 888
888  \"Y88b d88P  Y88b      d88888 888
888    888 Y88b.          d88P888 888
888    888  \"Y888b.      d88P 888 888
888    888     \"Y88b.   d88P  888 888
888    888       \"888  d88P   888 888
888  .d88P Y88b  d88P d8888888888 888
8888888P\"   \"Y8888P\" d88P     888 88888888
")

project("Data Structures and Algorithms Library (DSAL)"
        VERSION 0.1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(DSAL INTERFACE)
target_include_directories(DSAL INTERFACE src)
target_compile_options(DSAL INTERFACE $<$<COMPILE_LANGUAGE:CXX>:-O3>)

# ------------------------------------------------------------------------------
# Tests (Catch2)
# ------------------------------------------------------------------------------
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.1
)

FetchContent_MakeAvailable(Catch2)

# ------------------------------------------------------------------------------
# Tests (Runner)
# ------------------------------------------------------------------------------
enable_testing()

add_executable(runner test/main.cpp)
target_link_libraries(runner Catch2::Catch2)

target_sources(runner PRIVATE test/bigint.cpp)
target_link_libraries(runner DSAL)

# Run all tests which are not hidden.
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)

include(CTest)
include(Catch)

catch_discover_tests(runner)