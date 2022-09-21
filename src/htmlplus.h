#pragma once
#include "Palpatine.h"
#include <iostream>

namespace HMTLPLUS {
void header(std::ostream &out, std::string title) {
  out << R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>)"
      << title << "</title>\n";
  // for (auto &ss : Palpatine::get_stylesheet())
  //   out << R"(    <link rel="stylesheet" href=")" << ss << R"(">)" <<
  //   std::endl;
  // out << R"(</head>)";
  // for (auto &ss : Palpatine::stylesheet)
  //   out << R"(    <link rel="stylesheet" href=")" << ss << R"(">)" <<
  //   std::endl;
  // out << R"(</head>)";
}
} // namespace HMTLPLUS
