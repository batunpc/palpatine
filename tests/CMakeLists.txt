if(ENABLE_TESTING)
  set(TEST_MAIN
    "unit_tests")
  set(TEST_SOURCES
    "main.cpp")

  # target_include_directories(${PALPATINE_LIB} PUBLIC
  # ${CMAKE_CURRENT_SOURCE_DIR} # same as " ./ " # the header files
  # ${CMAKE_BINARY_DIR}/configured_files/include
  # ${PROJECT_SOURCE_DIR}/external/include # maddy's header files - parser lib for md
  # )
  add_executable(${TEST_MAIN} ${TEST_SOURCES})
  target_link_libraries(${TEST_MAIN} PUBLIC
    ${PALPATINE_LIB}
    argparse
    Catch2::Catch2WithMain
  )
endif(ENABLE_TESTING)
