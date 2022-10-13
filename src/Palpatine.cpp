#include "Palpatine.h"
#include "File.h"
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
  } else if (fs::path(input).extension() == ".txt") {
    string title = fs::path(input).stem().string();
    string file_content = file_sdds::read_file(input);

    std::vector<string> paragraphs;
    auto two_newline_position = file_content.find("\n\n\n");
    std::size_t last_blank_line_position = 0;

    // Remove the first line if it's a title, retrieve the title
    if (two_newline_position != string::npos) {
      title = file_content.substr(0, two_newline_position);
      last_blank_line_position = two_newline_position + 3;
    }
    // Extract the data into paragraphs
    while (last_blank_line_position < file_content.size() &&
           last_blank_line_position != string::npos) {
      std::size_t next_blank_line_position =
          file_content.find("\n\n", last_blank_line_position);
      if (next_blank_line_position == string::npos)
        break;
      paragraphs.push_back(file_content.substr(last_blank_line_position,
                                               next_blank_line_position -
                                                   last_blank_line_position));
      last_blank_line_position = next_blank_line_position + 2;
    }
    paragraphs.push_back(file_content.substr(last_blank_line_position));
    generate_page_file((fs::path(output) / name).string(), title, paragraphs);
  } else if (fs::path(input).extension() == ".md") {
    string title = fs::path(input).stem().string();
    string file_content = file_sdds::read_file(input);

    std::vector<string> paragraphs;
    std::size_t last_blank_line_position = 0;

    // Extract the data into paragraphs
    while (last_blank_line_position < file_content.size() &&
           last_blank_line_position != string::npos) {
      std::size_t next_blank_line_position =
          file_content.find("\n\n", last_blank_line_position);
      if (next_blank_line_position == string::npos)
        break;

      paragraphs.push_back(file_content.substr(last_blank_line_position,
                                               next_blank_line_position -
                                                   last_blank_line_position));
      last_blank_line_position = next_blank_line_position + 2;
    }

    paragraphs.push_back(file_content.substr(last_blank_line_position));

    std::regex link_regex(R"(\[([^\]]*)\]\(([^\)]*)\))");
    std::regex image_regex(R"(\!\[([^\]]*)\]\(([^\)]*)\))");
    std::regex hr_regex(R"(---)");
    std::regex inline_code_regex(R"((\s)`((?:[^`\\]|\\.)*)`)");

    /* for images */
    for (auto &paragraph : paragraphs) {
      std::smatch match;
      while (std::regex_search(paragraph, match, image_regex)) {
        string replacement = R"(<img src=")" + match[2].str() + R"(" alt=")" +
                             match[1].str() + R"(">)";
        paragraph.replace(match.position(), match.length(), replacement);
      }
    }
    for (auto &paragraph : paragraphs) {
      paragraph =
          std::regex_replace(paragraph, link_regex, "<a href=\"$2\">$1</a>");
      paragraph =
          std::regex_replace(paragraph, inline_code_regex, "$1<code>$2</code>");
      paragraph = std::regex_replace(paragraph, hr_regex, "\n<hr>\n");
    }
    generate_page_file((fs::path(output) / name).string(), title, paragraphs);
  } else {
    std::cout << "Error: " << input << " is not a valid file type" << std::endl;
    std::terminate();
  }
}

void Palpatine::generate_page_file(string output, string title,
                                   vector<string> paragraphs) {
  ofstream html(output);
  HMTLPLUS::header(html, title, this->stylesheets);
  HMTLPLUS::page_body(html, paragraphs);
}

void Palpatine::generate_index_file(string output, string title,
                                    vector<string> directories,
                                    vector<string> files) {
  ofstream html(output);
  HMTLPLUS::header(html, title, this->stylesheets);
  HMTLPLUS::index_body(html, title, directories, files);
}
