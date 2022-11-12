if(ENABLE_TESTING)
  set(TEST_MAIN
    "unit_tests")
  set(TEST_SOURCES
    "main.cpp")

SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "--coverage")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )




add_executable(${TEST_MAIN} ${TEST_SOURCES})

target_link_libraries(${TEST_MAIN} PUBLIC
${PALPATINE_LIB}
argparse
Catch2::Catch2WithMain
)
add_test(NAME ${TEST_MAIN} COMMAND ${TEST_MAIN})
endif(ENABLE_TESTING)
