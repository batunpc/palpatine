#include "Palpatine.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <vector>

#include "FileHandler.h"
#include "htmlplus.h"

void Palpatine::generate() { process_path(m_input, m_output, "index.html"); }

void Palpatine::process_path(const std::string& input,
                             const std::string& output,
                             const std::string& name) {
  if (std::filesystem::is_directory(input)) {
    // Store directories and files in current folder
    std::vector<std::string> directories;
    std::vector<std::string> files;

    for (const auto& entry : std::filesystem::directory_iterator(input)) {
      std::filesystem::path input_path = std::filesystem::path(entry);
      if (entry.is_directory()) {
        auto output_path =
            std::filesystem::path(output) / input_path.filename();
        // Create the directory
        std::filesystem::create_directory(output_path);
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
      std::string title =
          std::filesystem::relative(input, this->m_input).string();
      if (title == ".") {
        title = "Homepage";
      }
      generate_index_file((std::filesystem::path(output) / name).string(),
                          directories, title, files);
    }
  } else if (std::filesystem::path(input).extension() == ".txt" ||
             std::filesystem::path(input).extension() == ".md") {
    Handler* handler = std::filesystem::path(input).extension() == ".txt"
                           ? new TextHandler(stylesheets)
                           : new MarkdownHandler(stylesheets);
    handler->process(input, output, name);
    delete handler;
  } else {
    std::cout << "Error: " << input << " is not a valid file type" << std::endl;
    std::terminate();
  }
}

void Palpatine::generate_index_file(const std::string& out,
                                    const std::vector<std::string>& directories,
                                    const std::string& title,
                                    const std::vector<std::string>& files) {
  std::ofstream html(out);
  HMTLPLUS::header(html, title, this->stylesheets);
  HMTLPLUS::index_body(html, title, directories, files);
}
