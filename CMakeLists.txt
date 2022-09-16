cmake_minimum_required(VERSION 3.0.0)

project(palpatine
  VERSION 0.0.1
  DESCRIPTION "CLI static site generator."
  HOMEPAGE_URL "https://github.com/batunpc/palpatine"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PALPATINE_LIB "Libraries")
set(PALPATINE_EXE "Executable")

# Call the addGitSubmodule function to add the library
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(AddGitSubmodule) # This will include cmake module files

# Dependencies
add_git_submodule("external/parser")
add_git_submodule("external/termcolor")

add_subdirectory(config)
add_subdirectory(src)
add_subdirectory(app) # The executable lays here
