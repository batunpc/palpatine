#pragma once
#include <iostream>

namespace utils_sdds {
void print_banner();
void print_error(const std::runtime_error &err);
void print_location(const std::string &location);

} // namespace utils_sdds
