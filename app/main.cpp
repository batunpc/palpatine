#include "Palpatine.h"
#include "config.hpp"
#include <argparse/argparse.hpp>
#include <iostream>
#include <termcolor/termcolor.hpp>

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

void print_banner() {
  std::system("clear");
  std::cout << termcolor::on_grey << termcolor::bold << termcolor::white
            << " palpatine " << termcolor::reset << " v" << project_version
            << termcolor::dark << termcolor::reset << termcolor::bold
            << " Made with " << termcolor::red << "❤" << termcolor::reset
            << " by "
            << "Batuhan Ipci" << termcolor::reset << std::endl;
  // description
  std::cout << termcolor::dark << project_description << termcolor::reset
            << std::endl;
  std::cout << std::endl;
}

int main(int argc, char const *argv[]) {
  print_banner();
  auto program = setup_parser();
  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cerr << termcolor::red << termcolor::bold << err.what() << std::endl
              << termcolor::reset;
    std::cerr << program;
    std::exit(1);
  }

  auto output = program.get("-o").c_str();
  auto input = program.get("-i").c_str();
  auto style = program.get("-s").c_str();

  Palpatine palpatine(output, input, style);
  palpatine.generate();

  std::cout << termcolor::green << "Created at: " << termcolor::reset << output
            << "/dist" << std::endl;

  return 0;
}
