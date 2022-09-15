#include "Palpatine.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;
using std::string, std::vector, std::ifstream, std::ofstream, std::endl;

// Constructor
Palpatine::Palpatine(const char *output, const char *input,
                     const char *stylesheet) {
  // paths to the input and output directories
  this->output = (fs::path(output) / "dist").string();
  this->input = input;
  // default stylesheet
  this->stylesheet = std::vector{string("./style.css")};
  // if a stylesheet is provided, add it to the stylesheet array
  if (stylesheet != NULL) {
    this->stylesheet.push_back(stylesheet);
  }
  // remove the output directory if it exists
  fs::remove_all(this->output);
  // create the output directory if it doesn't exist
  fs::create_directory(this->output);
  // create default stylesheet file
  this->create_stylesheet_file();
}

void Palpatine::generate() { process_path(input, output, "index.html"); }

void Palpatine::create_stylesheet_file() {
  string css = R"(
        ul { 
            list-style: none; 
            padding: 0;
            margin: 0;
        }

        li { 
            padding-left: 1rem; 
            text-indent: -0.75rem;
            margin-top: 0.5rem;
        }

        li.folder::before {
            content: "📁 ";
        }

        li.file::before {
            content: "📄 ";
        }

        a { 
            text-decoration: none; 
            color: #008080; 
        }

        a:hover { 
            text-decoration: dotted underline; 
        }

        body { 
            background-color: #bbb; 
        }

        div.titlebar {
            background-color: #008080;
            color: #fff;
            padding: 3px;
            position: fixed;
            z-index: 10;
            top: 0;
            left: 0;
            right: 0;
            border-bottom: 1px solid #fff;
            height: 80px;
        }

        h1 { 
            font-weight: normal;
            text-align: center; 
            font-family: 'Brush Script MT', cursive; 
        }

        div.content { 
            max-width: 920px; font-family: Arial, sans-serif; 
            padding: 20px; 
            border: 1px solid #111; 
            background-color: #fff; 
            margin-left: auto; 
            margin-right: auto;
            margin-top: 100px;
            margin-bottom: 20px;
        }
        )";
  std::ofstream css_out_file(fs::path(output) / "style.css");
  css_out_file << css << std::endl;
  css_out_file.close();
}

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
      } else if (entry.is_regular_file() && in_path.extension() == ".txt") {
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
    {
      // Read file contents
      ifstream file_data(input);
      std::stringstream str_stream;
      str_stream << file_data.rdbuf();
      file_str = str_stream.str();
      file_data.close();
    }

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
  }
}

void Palpatine::generate_page_file(string output, string title,
                                   vector<string> paragraphs) {
  ofstream html(output);
  html << R"(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>)"
       << title << "</title>\n";
  for (auto &ss : stylesheet)
    html << R"(<link rel="stylesheet" href=")" << ss << R"(">)" << endl;
  html << R"(</head>
        <body>
            <div class="titlebar">
                <h1>)"
       << title << R"(</h1>
            </div>
            <div class="content">)";
  for (auto &p : paragraphs)
    html << "<p>" << p << "</p>" << endl;
  html << R"(</div>
        </body>
        </html>)";
  html.close();
}

void Palpatine::generate_index_file(string output, string title,
                                    vector<string> directories,
                                    vector<string> files) {
  ofstream html(output);
  html << R"(
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>)"
       << title << "</title>\n";
  for (auto &ss : stylesheet)
    html << R"(<link rel="stylesheet" href=")" << ss << R"(">)" << endl;
  html << R"(</head>
        <body>
            <div class="titlebar">
                <h1>)"
       << title << R"(</h1>
            </div>
            <div class="content">
                <ul>)";
  for (auto &dir : directories)
    html << R"(<li class="folder"><a href=")" << dir << R"(/">)" << dir
         << "</a></li>" << endl;
  for (auto &file : files)
    html << R"(<li class="file"><a href=")" << file << R"(.html">)" << file
         << "</a></li>" << endl;
  html << R"(</ul>
            </div>
        </body>
        </html>)";
  html.close();
}
