#pragma once
#include <iostream>

#include "Palpatine.h"

namespace HMTLPLUS {
void header(std::ostream& html_file, const std::string& title,
            const std::vector<string>& stylesheets) {
  html_file << R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>)"
            << title << "</title>\n";
  html_file << R"(
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/animate.css/4.1.1/animate.min.css">
    <script src="https://kit.fontawesome.com/7c3efb728f.js" crossorigin="anonymous"></script>
    <style> ul { list-style-type: none; margin: 0; padding: 0; overflow: hidden;} </style>
    )";

  for (const auto& stylesheet : stylesheets) {
    html_file << R"(
    <link rel="stylesheet" href=")"
              << stylesheet << R"(">)" << std::endl;
  }
  html_file << R"(</head>)";
  html_file << R"(
<body>
    <div>)";
  if (title == "Homepage") {
    html_file << R"(
        <h1> <i class="fa-solid fa-house"></i>)"
              << R"(</h1>
    </div>)";
  } else {
    html_file << R"(
        <h1 class="animate__animated animate__fadeInLeft">)"
              << title << R"(</h1>
    </div>)";
  }
}

void page_body(std::ostream& html_file, const std::vector<string>& paragraphs) {
  for (const auto& paragraph : paragraphs) {
    // if image is found do not add <p> tag
    if (paragraph.find("<img") != std::string::npos) {
      html_file << R"(
        <div>)" << paragraph
                << R"(</div>)";
    } else {
      html_file << R"(
        <p>)" << paragraph
                << R"(</p>)" << std::endl;
    }
  }
  html_file << R"(
  </body>
</html>)";
}

void footer(std::ostream& html_file) {
  html_file << R"(
  </body>
</html>)";
}

void index_body(std::ostream& html_file, const std::string& title,
                const std::vector<string>& directories,
                const std::vector<string>& files) {
  if (title == "Homepage") {
    html_file << R"(
        <h2>Directories</h2>
        <ul>)";
    for (const auto& dir : directories) {
      html_file << R"(<li><a href=")" << dir
                << R"("/><i class="fas fa-folder"></i> )" << dir
                << R"(</a></li>)";
    }
  } else {
    html_file << R"(
        <h2>Files</h2>
        <ul>)";
    for (const auto& file : files) {
      html_file << R"(<li><a href=")" << file
                << R"(.html"><i class="fa-regular fa-file-lines"></i> )" << file
                << R"(</a></li>)";
    }
  }
  html_file << R"(
        </ul>
</body>
</html>)";
}
}  // namespace HMTLPLUS
