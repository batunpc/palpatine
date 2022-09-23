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

  for (auto &ss : stylesheet)
    out << R"(
    <link rel="stylesheet" href=")"
        << ss << R"(">)" << std::endl;
  out << R"(</head>)";
}

void page_body(std::ostream &out, std::string title,
               std::vector<string> paragraphs) {
  out << R"(
<body>
    <div>
        <h1 class="animate__animated animate__bounce">)"
      << title << R"(</h1>
    </div>
    <div>)";
  for (auto &p : paragraphs)
    out << R"(<p>)" << p << R"(</p>)";
  out << R"(
    </div>
</body>
</html>)";
}
} // namespace HMTLPLUS
