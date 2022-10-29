cmake_minimum_required(VERSION 3.0.0)

project(palpatine
  VERSION 1.0.0
  DESCRIPTION "CLI static site generator."
  HOMEPAGE_URL "https://github.com/batunpc/palpatine"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PALPATINE_LIB "Libraries")
set(PALPATINE_EXE "palpatine")
set(CMAKE_BUILD_TYPE "Debug")

# Call the addGitSubmodule function to add the library
# set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/custom-modules ${CMAKE_MODULE_PATH})

include(AddGitSubmodule)

# Dependencies
add_git_submodule("external/parser")
add_git_submodule("external/termcolor")
add_git_submodule("external/json")

add_subdirectory(config)
add_subdirectory(src)
add_subdirectory(app) # The executable lays here
