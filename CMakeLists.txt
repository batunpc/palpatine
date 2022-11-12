cmake_minimum_required(VERSION 3.14)

project(palpatine
  VERSION 0.5.9
  DESCRIPTION "CLI static site generator."
  HOMEPAGE_URL "https://github.com/batunpc/palpatine"
  LANGUAGES CXX
)

# Global CMake variables
# ----------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED            ON)
set(CMAKE_CXX_EXTENSIONS                  OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS          ON)

# Options
# -------
option(ENABLE_CLANG_TIDY "Enable to add clang tidy."              OFF)
option(ENABLE_TESTING "Unit testing build"                        ON)
option(CODE_COVERAGE "Collect coverage from test library"         OFF)

# Naming conventions for targets lib and exe
# ------------------------------------------
set(PALPATINE_LIB "Libraries")
set(PALPATINE_EXE "palpatine")
set(CMAKE_BUILD_TYPE "Debug")

# CMakes modules
# --------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

include(AddGitSubmodule)
include(StaticAnalyzers) # clang-tidy

# External libraries - dependencies of palpatine
# ---------------------------------------------
# include(FetchContent)
add_git_submodule("external/parser")
add_git_submodule("external/termcolor")
add_git_submodule("external/json")

# Catch2 testing framework
add_git_submodule("external/catch2")

# Subdirectories
# --------------
add_subdirectory(config)
add_subdirectory(src)
add_subdirectory(app) # The executable lays here

# Testing
# -------
add_subdirectory(tests)
