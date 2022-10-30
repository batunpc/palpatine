#include "Utils.h"
#include "config.h"
#include <iostream>
#include <termcolor/termcolor.hpp>

namespace utils_sdds {

  void print_banner() {
    std::system("clear");
    std::cout << termcolor::on_grey << termcolor::bold << termcolor::white
              << project_name << termcolor::reset << " v" << project_version
              << termcolor::dark << termcolor::reset << termcolor::bold
              << " Made with " << termcolor::red << "❤" << termcolor::reset
              << " by "
              << "batunpc" << termcolor::reset << std::endl;
    // description
    std::cout << termcolor::dark << project_description << termcolor::reset
              << std::endl;
    std::cout << std::endl;
  }

  void print_error(const std::string& err) {
    std::cerr << termcolor::red << termcolor::bold << err << termcolor::reset
              << std::endl;
  }

  void print_location(const std::string& location) {
    std::cout << termcolor::green << "Created at: " << termcolor::reset
              << location << "/dist" << std::endl;
  }
} // namespace utils_sdds
