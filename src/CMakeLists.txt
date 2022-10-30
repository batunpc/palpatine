add_library(${PALPATINE_LIB} STATIC
  "Palpatine.cpp"
  "File.cpp"
  "Utils.cpp")

target_include_directories(${PALPATINE_LIB} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR} # same as " ./ " # the header files
  ${CMAKE_BINARY_DIR}/configured_files/include
  ${PROJECT_SOURCE_DIR}/external/include # maddy's header files - parser lib for md
)
target_link_libraries(${PALPATINE_LIB} PUBLIC
  termcolor
)
