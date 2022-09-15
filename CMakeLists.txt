cmake_minimum_required(VERSION 3.0.0)

project(palpatine
  VERSION 1.0.0
  DESCRIPTION "Static site generator (SSG) for generating static websites from raw data "
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
add_git_submodule("external/parser")

add_subdirectory(config)
add_subdirectory(src)
add_subdirectory(app) # The executable lays here
