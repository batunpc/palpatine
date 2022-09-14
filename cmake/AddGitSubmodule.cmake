
function(add_git_submodule dir)
  find_package(Git REQUIRED) # Ensure git is installed

  if(NOT EXISTS ${dir}/CMakeLists.txt)
    message(STATUS "Adding git submodule ${dir}")
    execute_process(COMMAND ${GIT_EXECUTABLE}
      submodule update --init --recursive -- ${dir}
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}) # Absolute path to project root
  endif()

  add_subdirectory(${dir})
endfunction(add_git_submodule)
