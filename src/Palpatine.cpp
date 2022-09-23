#include "Palpatine.h"
#include "htmlplus.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;
using std::string, std::vector, std::ifstream, std::ofstream;

// Constructor
Palpatine::Palpatine(const char *output, const char *input,
                     const char *stylesheet) {
  // paths to the input and output directories
  this->output = (fs::path(output) / "dist").string();
  this->input = input;
  // animated stylesheet
  this->stylesheet =
      std::vector{string("https://cdnjs.cloudflare.com/ajax/libs/animate.css/"
                         "4.1.1/animate.min.css")};

  if (stylesheet != NULL) {
    this->stylesheet.push_back(stylesheet);
  }
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
      fs::path in_path = fs::path(entry);
      if (entry.is_directory()) {
        auto out_path = fs::path(output) / in_path.filename();
        // Create the directory
        fs::create_directory(out_path);

        // Create the index file for the directory
        process_path(in_path.string(), out_path.string(), "index.html");

        directories.push_back(in_path.filename().string());
      } else if (entry.is_regular_file() && (in_path.extension() == ".txt" ||
                                             in_path.extension() == ".md")) {
        // Create the page file
        process_path(in_path.string(), output,
                     in_path.stem().string() + ".html");

        files.push_back(in_path.stem().string());
      }

      string title = fs::relative(input, this->input).string();
      if (title == ".")
        title = "Homepage";

      generate_index_file((fs::path(output) / name).string(), title,
                          directories, files);
    }
  } else if (fs::path(input).extension() == ".txt") {
    std::string title = fs::path(input).stem().string();
    string file_str;

    // Read file contents
    ifstream file_data(input);
    std::stringstream str_stream;
    str_stream << file_data.rdbuf();
    file_str = str_stream.str();
    file_data.close();

    std::vector<string> paragraphs;
    auto two_newline = file_str.find("\n\n\n");
    std::size_t last_blank_line = 0;

    // Remove the first line if it's a title, retrieve the title
    if (two_newline != string::npos) {
      title = file_str.substr(0, two_newline);
      last_blank_line = two_newline + 3;
    }

    // Extract the data into paragraphs
    while (last_blank_line < file_str.size() &&
           last_blank_line != string::npos) {
      std::size_t next_blank_line = file_str.find("\n\n", last_blank_line);
      if (next_blank_line == string::npos)
        break;

      paragraphs.push_back(
          file_str.substr(last_blank_line, next_blank_line - last_blank_line));
      last_blank_line = next_blank_line + 2;
    }
    generate_page_file((fs::path(output) / name).string(), title, paragraphs);
  } else if (fs::path(input).extension() == ".md") {
    std::string title = fs::path(input).stem().string();
    string file_str;

    // Read file contents
    ifstream file_data(input);
    std::stringstream str_stream;
    str_stream << file_data.rdbuf();
    file_str = str_stream.str();
    file_data.close();

    std::vector<string> paragraphs;
    std::size_t last_blank_line = 0;

    // Extract the data into paragraphs
    while (last_blank_line < file_str.size() &&
           last_blank_line != string::npos) {
      std::size_t next_blank_line = file_str.find("\n\n", last_blank_line);
      if (next_blank_line == string::npos)
        break;

      paragraphs.push_back(
          file_str.substr(last_blank_line, next_blank_line - last_blank_line));
      last_blank_line = next_blank_line + 2;
    }

    paragraphs.push_back(file_str.substr(last_blank_line));

    std::regex link(R"(\[([^\]]*)\]\(([^\)]*)\))");
    std::regex image(R"(\!\[([^\]]*)\]\(([^\)]*)\))");

    /* for images */
    for (auto &paragraph : paragraphs) {
      std::smatch match;
      while (std::regex_search(paragraph, match, image)) {
        std::string replacement = R"(<img src=")" + match[2].str() +
                                  R"(" alt=")" + match[1].str() + R"(">)";
        paragraph.replace(match.position(), match.length(), replacement);
      }
    }
    for (auto &paragraph : paragraphs)
      paragraph = std::regex_replace(paragraph, link, "<a href=\"$2\">$1</a>");
    generate_page_file((fs::path(output) / name).string(), title, paragraphs);
  } else {
    std::cout << "Error: " << input << " is not a valid file type" << std::endl;
    std::terminate();
  }
}

void Palpatine::generate_page_file(string output, string title,
                                   vector<string> paragraphs) {
  ofstream html(output);
  HMTLPLUS::header(html, title, this->stylesheet);
  HMTLPLUS::page_body(html, title, paragraphs);
}

void Palpatine::generate_index_file(string output, string title,
                                    vector<string> directories,
                                    vector<string> files) {
  ofstream html(output);
  HMTLPLUS::header(html, title, this->stylesheet);
  html << R"(<body>
    <div>
        <h1>)"
       << title << R"(</h1>
    </div>
    <div>
        <ul>)";
  for (auto &dir : directories)
    html << R"(<li><a href=")" << dir << R"(/">)" << dir << "</a></li>";
  for (auto &file : files)
    html << R"(
          <li><a href=")"
         << file << R"(.html">)" << file << "</a></li>";
  html << R"(
        </ul>
    </div>
</body>
</html>)";
  html.close();
}
