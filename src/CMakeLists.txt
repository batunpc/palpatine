add_library(${PALPATINE_LIB} STATIC Palpatine.cpp)

target_include_directories(${PALPATINE_LIB} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR} # same as "./"
  ${CMAKE_BINARY_DIR}/configured_files/include
)
