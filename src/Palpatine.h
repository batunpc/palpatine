#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#pragma once

class Palpatine {
 public:
  Palpatine(const char* output, const char* input, const char* stylesheet)
      : m_input(input) {
    // paths to the input and output directories
    this->m_output = (std::filesystem::path(output) / "dist").string();
    this->stylesheets = std::vector{std::string("")};
    if (stylesheet != nullptr) {
      this->stylesheets.emplace_back(stylesheet);
    }
    // remove the output directory if it exists
    std::filesystem::remove_all(this->m_output);
    // create the output directory if it doesn't exist
    std::filesystem::create_directory(this->m_output);
  }

  void generate();

 private:
  std::string m_input, m_output;

  std::vector<std::string> stylesheets;

  void process_path(const std::string&, const std::string&, const std::string&);
  void generate_index_file(const std::string&, const std::vector<std::string>&,
                           const std::string&, const std::vector<std::string>&);
};
