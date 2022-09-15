add_executable(${PALPATINE_EXE} "main.cpp")
target_link_libraries(${PALPATINE_EXE} PUBLIC
  ${PALPATINE_LIB}
  argparse
  termcolor
)
