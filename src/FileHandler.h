#pragma once
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <utility>
#include <vector>

#include "htmlplus.h"
#include "maddy/parser.h"

class Handler {
 public:
  explicit Handler(const std::vector<std::string>& stylesheets)
      : stylesheets(stylesheets) {}

  virtual void process(std::string input, std::string output,
                       std::string name) = 0;

  virtual ~Handler() = default;

 protected:
  void generate_page_file(const std::string& output, const std::string& title,
                          const std::vector<std::string>& paragraphs) {
    std::ofstream html(output);
    HMTLPLUS::header(html, title, stylesheets);
    HMTLPLUS::page_body(html, paragraphs);
  }

  const std::vector<std::string>& stylesheets;

  static auto read_file(const std::string& file_name) -> std::string {
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
  explicit TextHandler(const std::vector<std::string>& stylesheets)
      : Handler(stylesheets) {}

  ~TextHandler() override = default;

  virtual void process(std::string input, std::string output,
                       std::string name) {
    std::string title = std::filesystem::path(input).stem().string();
    std::string file_content = read_file(input);
    auto two_newline_position = file_content.find("\n\n\n");
    std::size_t last_blank_line_position = 0;

    // Remove the first line if it's a title, retrieve the title
    if (two_newline_position != std::string::npos) {
      title = file_content.substr(0, two_newline_position);

      last_blank_line_position = two_newline_position + 3;
    }

    std::vector<std::string> paragraphs =
        parse_paragraphs(file_content, last_blank_line_position);
    generate_page_file((std::filesystem::path(output) / name).string(), title,
                       paragraphs);
  }

 protected:
  virtual auto parse_paragraphs(const std::string& file_content,
                                std::size_t start) -> std::vector<std::string> {
    static const std::string separator = "\n\n";
    std::vector<std::string> paragraphs;
    std::size_t last_blank_line_position = start;

    // Extract the data into paragraphs
    while (last_blank_line_position < file_content.size() &&
           last_blank_line_position != std::string::npos) {
      std::size_t next_blank_line_position =
          file_content.find(separator, last_blank_line_position);
      if (next_blank_line_position == std::string::npos) {
        break;
      }

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
  explicit MarkdownHandler(const std::vector<std::string>& stylesheets)
      : TextHandler(stylesheets) {}

  ~MarkdownHandler() override = default;

  void process(std::string input, std::string output,
                       std::string name) override {
    std::string title = std::filesystem::path(input).stem().string();
    std::ifstream file_data(input);
    std::stringstream md_str;
    md_str << file_data.rdbuf();
    file_data.close();

    std::shared_ptr<maddy::Parser> parser = std::make_shared<maddy::Parser>();
    std::string html_output = parser->Parse(md_str);

    std::ofstream html(std::filesystem::path(output) / name);
    HMTLPLUS::header(html, title, stylesheets);
    html << html_output << std::endl;
    HMTLPLUS::footer(html);
  }
};
