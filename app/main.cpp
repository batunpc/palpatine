#include "Palpatine.h"
#include "Utils.h"
#include <algorithm>
#include <argparse/argparse.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>

argparse::ArgumentParser setup_parser(const std::vector<std::string>& argv) {
  argparse::ArgumentParser program("palpatine");

  program.add_argument("-o", "--output")
      .default_value(std::string("."))
      .help("The output directory (where dist folder will be created)");
  program.add_argument("-s", "--stylesheet")
      .default_value(std::string(
          "https://cdn.jsdelivr.net/gh/kimeiga/bahunya/dist/bahunya.min.css"))
      .help("The stylesheet file link");
  program.add_argument("-c", "--config").help("The config JSON file");

  if (!(std::count(argv.begin(), argv.end(), "-c") or
        std::count(argv.begin(), argv.end(), "--config"))) {
    program.add_argument("-i", "--input")
        .required()
        .help("The input file / directory");
  } else {
    program.add_argument("-i", "--input").help("The input file / directory");
  }

  return program;
}

int main(int argc, char const* argv[]) {
  utils_sdds::print_banner();

  std::vector<std::string> args(argv, argv + argc);
  auto                     program = setup_parser(args);

  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error& err) {
    utils_sdds::print_error(err.what());
    std::cerr << program;
    std::exit(1);
  }
  std::map<std::string, std::string> options;

  options["output"]     = program.get("-o");
  options["stylesheet"] = program.get("-s");

  if (auto p_value = program.present("-c")) {
    std::ifstream json_stream(*p_value);
    if (json_stream.fail()) {
      std::cerr << "Configuration file not found at: " << *p_value << std::endl;
      std::exit(1);
    }
    nlohmann::json config_json;
    json_stream >> config_json;

    if (config_json.find("input") == config_json.end()) {
      std::cerr << "Input file / directory not specified in config file"
                << std::endl;
      std::exit(1);
    }

    if (auto input_file_optional = program.present("-i"))
      options["input"] = *input_file_optional;

    for (auto& entry : config_json.items())
      options[entry.key()] = entry.value();
  } else {
    options["input"] = program.get("-i");
  }

  Palpatine palpatine(
      options["output"].c_str(), options["input"].c_str(),
      options["stylesheet"].c_str());
  palpatine.generate();

  utils_sdds::print_location(options["output"]);

  return 0;
}
