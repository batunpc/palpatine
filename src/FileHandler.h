#pragma once
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

class Handler {
public:
  Handler(const vector<string> &stylesheets) : stylesheets(stylesheets) {}

  virtual ~Handler() {}

  virtual void process(string input, string output, string name) = 0;

protected:
  void generate_page_file(string output, string title,
                          vector<string> paragraphs) {
    ofstream html(output);
    HMTLPLUS::header(html, title, stylesheets);
    HMTLPLUS::page_body(html, paragraphs);
  }

private:
  const vector<string> &stylesheets;
};

class TextHandler : public Handler {

public:
  TextHandler(const vector<string> &stylesheets) : Handler(stylesheets) {}

  virtual ~TextHandler() {}

  virtual void process(string input, string output, string name) {
    string title = fs::path(input).stem().string();
    string file_content = file_sdds::read_file(input);
    auto two_newline_position = file_content.find("\n\n\n");
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
  virtual std::vector<string> parse_paragraphs(const string &file_content,
                                               std::size_t start) {
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

      paragraphs.push_back(file_content.substr(last_blank_line_position,
                                               next_blank_line_position -
                                                   last_blank_line_position));
      last_blank_line_position = next_blank_line_position + separator.size();
    }

    paragraphs.push_back(file_content.substr(last_blank_line_position));
    return paragraphs;
  }
};

class MarkdownHandler : public TextHandler {

public:
  MarkdownHandler(const vector<string> &stylesheets)
      : TextHandler(stylesheets) {}

  virtual ~MarkdownHandler() {}

  virtual void process(string input, string output, string name) {
    string title = fs::path(input).stem().string();
    string file_content = file_sdds::read_file(input);
    std::vector<string> paragraphs = parse_paragraphs(file_content, 0);

    static const std::regex link_regex(R"(\[([^\]]*)\]\(([^\)]*)\))");
    static const std::regex image_regex(R"(\!\[([^\]]*)\]\(([^\)]*)\))");
    static const std::regex hr_regex(R"(---)");
    static const std::regex inline_code_regex(R"((\s)`((?:[^`\\]|\\.)*)`)");

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
  }
};
