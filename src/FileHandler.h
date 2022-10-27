#pragma once
#include "htmlplus.h"
#include "maddy/parser.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <utility>
#include <vector>

namespace fs = std::filesystem;
using std::vector, std::ifstream, std::ofstream;

class Handler {
public:
  explicit Handler(const vector<string>& stylesheets)
      : stylesheets(stylesheets) {}

  virtual void process(string input, string output, string name) = 0;

  virtual ~Handler() = default;

protected:
  void generate_page_file(
      const string& output, const string& title,
      const vector<string>& paragraphs) {
    ofstream html(output);
    HMTLPLUS::header(html, title, stylesheets);
    HMTLPLUS::page_body(html, paragraphs);
  }

  const vector<string>& stylesheets;

  static auto read_file(const string& file_name) -> string {
    std::string file_str;
    std::ifstream file_data(file_name);
    std::stringstream str_stream;
    str_stream << file_data.rdbuf();
    file_str = str_stream.str();
    file_data.close();
    return file_str;
  }
};

class TextHandler : public Handler {
public:
  explicit TextHandler(const vector<string>& stylesheets)
      : Handler(stylesheets) {}

  virtual ~TextHandler() {}

  virtual void process(string input, string output, string name) {
    string title                         = fs::path(input).stem().string();
    string file_content                  = read_file(input);
    auto two_newline_position            = file_content.find("\n\n\n");
    std::size_t last_blank_line_position = 0;

    // Remove the first line if it's a title, retrieve the title
    if (two_newline_position != string::npos) {
      title = file_content.substr(0, two_newline_position);

      last_blank_line_position = two_newline_position + 3;
    }

    std::vector<string> paragraphs =
        parse_paragraphs(file_content, last_blank_line_position);
    generate_page_file((fs::path(output) / name).string(), title, paragraphs);
  }

protected:
  virtual auto parse_paragraphs(const string& file_content, std::size_t start)
      -> std::vector<string> {
    static const string separator = "\n\n";
    std::vector<string> paragraphs;
    std::size_t last_blank_line_position = start;

    // Extract the data into paragraphs
    while (last_blank_line_position < file_content.size() &&
           last_blank_line_position != string::npos) {
      std::size_t next_blank_line_position =
          file_content.find(separator, last_blank_line_position);
      if (next_blank_line_position == string::npos)
        break;

      paragraphs.push_back(file_content.substr(
          last_blank_line_position,
          next_blank_line_position - last_blank_line_position));
      last_blank_line_position = next_blank_line_position + separator.size();
    }
    paragraphs.push_back(file_content.substr(last_blank_line_position));
    return paragraphs;
  }
};

class MarkdownHandler : public TextHandler {
public:
  explicit MarkdownHandler(const vector<string>& stylesheets)
      : TextHandler(stylesheets) {}

  virtual ~MarkdownHandler() {}

  virtual void process(string input, string output, string name) {
    string title = fs::path(input).stem().string();
    std::ifstream file_data(input);
    std::stringstream md_str;
    md_str << file_data.rdbuf();
    file_data.close();

    std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>();
    std::string html_output               = parser->Parse(md_str);

    ofstream html(fs::path(output) / name);
    HMTLPLUS::header(html, title, stylesheets);
    html << html_output << std::endl;
    HMTLPLUS::footer(html);
  }
};
