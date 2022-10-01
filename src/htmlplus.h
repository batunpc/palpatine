#pragma once
#include "Palpatine.h"
#include <iostream>

namespace HMTLPLUS {
void header(std::ostream &out, std::string title,
            std::vector<string> stylesheet) {
  out << R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>)"
      << title << "</title>\n";
  out << R"(
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/animate.css/4.1.1/animate.min.css">
    <script src="https://kit.fontawesome.com/7c3efb728f.js" crossorigin="anonymous"></script>
    <style> ul { list-style-type: none; margin: 0; padding: 0; overflow: hidden;} </style>
    )";

  for (auto &ss : stylesheet)
    out << R"(
    <link rel="stylesheet" href=")"
        << ss << R"(">)" << std::endl;
  out << R"(</head>)";
  out << R"(
<body>
    <div>)";
  if (title == "Homepage") {
    out << R"(
        <h1> <i class="fa-solid fa-house"></i>)"
        << R"(</h1>
    </div>)";
  } else {
    out << R"(
        <h1 class="animate__animated animate__fadeInLeft">)"
        << title << R"(</h1>
    </div>)";
  }
}

void page_body(std::ostream &out, std::vector<string> paragraphs) {
  for (auto &p : paragraphs)
    // if image is found do not add <p> tag
    if (p.find("<img") != std::string::npos)
      out << R"(
        <div>)"
          << p << R"(</div>)";
    else
      out << R"(
        <p>)"
          << p << R"(</p>)" << std::endl;
  out << R"(
  </body>
</html>)";
}

void index_body(std::ostream &out, std::string title,
                std::vector<string> directories, std::vector<string> files) {
  if (title == "Homepage") {
    out << R"(
        <h2>Directories</h2>
        <ul>)";
    for (auto &d : directories)
      out << R"(<li><a href=")" << d << R"("/><i class="fas fa-folder"></i> )"
          << d << R"(</a></li>)";
  } else {
    out << R"(
        <h2>Files</h2>
        <ul>)";
    for (auto &f : files)
      out << R"(<li><a href=")" << f
          << R"(.html"><i class="fa-regular fa-file-lines"></i> )" << f
          << R"(</a></li>)";
  }
  out << R"(
        </ul>
</body>
</html>)";
}
} // namespace HMTLPLUS
