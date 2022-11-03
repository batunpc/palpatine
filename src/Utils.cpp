#include "Utils.h"

#include <iostream>
#include <termcolor/termcolor.hpp>

#include "config.h"

namespace utils_sdds {

void print_banner() {
  std::cout << "\x1B[2J\x1B[H";  // clears console
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

  // using https://patorjk.com/software/taag/
  //  std::cout << termcolor << R"(
  //        _____  _____  ____   _____  _____  ____  ___  _____  _____
  //        /  _  \/  _  \/  _/  /  _  \/  _  \/    \/___\/  _  \/   __\
    //       |   __/|  _  ||  |---|   __/|  _  |\-  -/|   ||  |  ||   __|
  //        \__/   \__|__/\_____/\__/   \__|__/ |__| \___/\__|__/\_____/

  //                                       )"
  //           << std::endl;
}

void print_error(const std::string& err) {
  std::cerr << termcolor::red << termcolor::bold << err << termcolor::reset
            << std::endl;
}

void print_location(const std::string& location) {
  std::cout << termcolor::green << "Created at: " << termcolor::reset
            << location << "/dist" << std::endl;
}
}  // namespace utils_sdds
