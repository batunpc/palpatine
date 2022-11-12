#include <algorithm>
#include <iostream>

#include "Palpatine.h"
#include "Utils.h"
#include "config.h"
#include "parser.h"

int main(int argc, char const* argv[]) {
  utils_sdds::print_banner();

  std::vector<std::string> args(argv, argv + argc);
  try {
    auto options = get_options(args);
    Palpatine palpatine(options["output"].c_str(), options["input"].c_str(),
                        options["stylesheet"].c_str());
    palpatine.generate();

    utils_sdds::print_location(options["output"]);
    return 0;
  } catch (const std::runtime_error& err) {
    utils_sdds::print_error(err.what());
    // std::cerr << program;
    return 1;
  }
}
