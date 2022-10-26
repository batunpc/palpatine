#include "Palpatine.h"
#include "File.h"
#include "FileHandler.h"
#include "Utils.h"
#include "htmlplus.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;
using std::vector, std::ifstream, std::ofstream;

// Constructor
Palpatine::Palpatine(const char *output, const char *input,
                     const char *stylesheet) {
  // paths to the input and output directories
  this->output = (fs::path(output) / "dist").string();
  this->input = input;
  this->stylesheets = std::vector{string("")};
  if (stylesheet != NULL)
    this->stylesheets.push_back(stylesheet);
  // remove the output directory if it exists
  fs::remove_all(this->output);
  // create the output directory if it doesn't exist
  fs::create_directory(this->output);
}

void Palpatine::generate() { process_path(input, output, "index.html"); }

void Palpatine::process_path(string input, string output, string name) {
  if (fs::is_directory(input)) {
    // Store directories and files in current folder
    std::vector<string> directories, files;
    for (auto entry : fs::directory_iterator(input)) {
      fs::path input_path = fs::path(entry);
      if (entry.is_directory()) {
        auto output_path = fs::path(output) / input_path.filename();
        // Create the directory
        fs::create_directory(output_path);
        // Create the index file for the directory
        process_path(input_path.string(), output_path.string(), "index.html");

        directories.push_back(input_path.filename().string());
      } else if (entry.is_regular_file() && (input_path.extension() == ".txt" ||
                                             input_path.extension() == ".md")) {
        // Create the page file
        process_path(input_path.string(), output,
                     input_path.stem().string() + ".html");

        files.push_back(input_path.stem().string());
      }
      string title = fs::relative(input, this->input).string();
      if (title == ".")
        title = "Homepage";
      generate_index_file((fs::path(output) / name).string(), title,
                          directories, files);
    }
  } else if (fs::path(input).extension() == ".txt" ||
             fs::path(input).extension() == ".md") {
    Handler *handler = fs::path(input).extension() == ".txt"
                           ? new TextHandler(stylesheets)
                           : new MarkdownHandler(stylesheets);
    handler->process(input, output, name);
    delete handler;
  } else {
    utils_sdds::print_error("Error: " + input + " is not a valid file type");
    std::terminate();
  }
}

void Palpatine::generate_index_file(string output, string title,
                                    vector<string> directories,
                                    vector<string> files) {
  ofstream html(output);
  HMTLPLUS::header(html, title, this->stylesheets);
  HMTLPLUS::index_body(html, title, directories, files);
}
