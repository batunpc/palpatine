#include "Palpatine.h"
#include "config.hpp"
#include <argparse/argparse.hpp>
#include <termcolor/termcolor.hpp>
#include <iostream>

argparse::ArgumentParser setup_parser() {
  argparse::ArgumentParser program("palpatine", "0.0.1");
  program.add_argument("-i", "--input")
      .required()
      .help("The input file / directory");

  program.add_argument("-o", "--output")
      .default_value(std::string("."))
      .help("The output directory (where dist folder will be created)");

  program.add_argument("-s", "--stylesheet")
      .default_value(std::string(""))
      .help("The stylesheet file link");

  return program;
}

void print_banner() {
  // Version and author
  std::cout << termcolor::on_green << termcolor::bold
            << " palpatine " << termcolor::reset << " v" << project_version << termcolor::dark << termcolor::reset << termcolor::bold
            << " Made with " << termcolor::red << "❤" << termcolor::reset << " by "
            << "Batuhan Ipci" << termcolor::reset << std::endl;
  // description
  std::cout << termcolor::dark
            << project_description << termcolor::reset << std::endl;
  //
  std::cout << std::endl;
}

int main(int argc, char const *argv[]) {
  print_banner();
  auto program = setup_parser();
  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }

  // Create the generator obj
  Palpatine static_site_gen(program.get("-o").c_str(),
                            program.present("-i").value().c_str(),
                            program.get("-s").c_str());
  static_site_gen.generate();

  return 0;
}
