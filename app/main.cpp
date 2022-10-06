#include "Palpatine.h"
#include "Utils.h"
#include <argparse/argparse.hpp>
#include <iostream>


argparse::ArgumentParser setup_parser() {
  argparse::ArgumentParser program("palpatine");
  program.add_argument("-i", "--input")
      .required()
      .help("The input file / directory");
  program.add_argument("-o", "--output")
      .default_value(std::string("."))
      .help("The output directory (where dist folder will be created)");
  program.add_argument("-s", "--stylesheet")
      .default_value(std::string(
          "https://cdn.jsdelivr.net/gh/kimeiga/bahunya/dist/bahunya.min.css"))
      .help("The stylesheet file link");
  return program;
}

int main(int argc, char const *argv[]) {
  utils_sdds::print_banner();

  auto program = setup_parser();
  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    utils_sdds::print_error(err);
    std::cerr << program;
    std::exit(1);
  }

  auto output = program.get("-o").c_str();
  auto input = program.get("-i").c_str();
  auto style = program.get("-s").c_str();

  Palpatine palpatine(output, input, style);
  palpatine.generate();

  utils_sdds::print_location(output);

  return 0;
}
