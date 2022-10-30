#include "Palpatine.h"
#include "File.h"
#include "FileHandler.h"
#include "htmlplus.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;
using std::vector, std::ifstream, std::ofstream;

// Constructor
Palpatine::Palpatine(
    const char* output, const char* input, const char* stylesheet)
    : m_input(input) {
  // paths to the input and output directories
  this->m_output    = (fs::path(output) / "dist").string();
  this->stylesheets = std::vector{string("")};
  if (stylesheet != nullptr) {
    this->stylesheets.emplace_back(stylesheet);
  }
  // remove the output directory if it exists
  fs::remove_all(this->m_output);
  // create the output directory if it doesn't exist
  fs::create_directory(this->m_output);
}



void Palpatine::generate() { process_path(m_input, m_output, "index.html"); }

void Palpatine::process_path(
    const string& input, const string& output, const string& name) {
  if (fs::is_directory(input)) {
    // Store directories and files in current folder
    std::vector<string> directories, files;

    for (const auto& entry : fs::directory_iterator(input)) {
      fs::path input_path = fs::path(entry);
      if (entry.is_directory()) {
        auto output_path = fs::path(output) / input_path.filename();
        // Create the directory
        fs::create_directory(output_path);
        // Create the index file for the directory
        process_path(input_path.string(), output_path.string(), "index.html");

        directories.push_back(input_path.filename().string());
      } else if (
          entry.is_regular_file() && (input_path.extension() == ".txt" ||
                                      input_path.extension() == ".md")) {
        // Create the page file
        process_path(
            input_path.string(), output, input_path.stem().string() + ".html");

        files.push_back(input_path.stem().string());
      }
      string title = fs::relative(input, this->m_input).string();
      if (title == ".") {
        title = "Homepage";
      }
      generate_index_file(
          (fs::path(output) / name).string(), title, directories, files);
    }
  } else if (
      fs::path(input).extension() == ".txt" ||
      fs::path(input).extension() == ".md") {
    Handler* handler = fs::path(input).extension() == ".txt"
                           ? new TextHandler(stylesheets)
                           : new MarkdownHandler(stylesheets);
    handler->process(input, output, name);
    delete handler;
  } else {
    std::cout << "Error: " << input << " is not a valid file type" << std::endl;
    std::terminate();
  }
}

void Palpatine::generate_index_file(
    const string& out, const string& title, const vector<string>& directories,
    const vector<string>& files) {
  ofstream html(out);
  HMTLPLUS::header(html, title, this->stylesheets);
  HMTLPLUS::index_body(html, title, directories, files);
}
