#include <algorithm>
#include <argparse/argparse.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>

auto get_options(const std::vector<std::string>& argv)
    -> std::map<std::string, std::string> {
  argparse::ArgumentParser program("palpatine", "0.5.9");

  program.add_argument("-o", "--output")
      .default_value(std::string("."))
      .help("The output directory (where dist folder will be created)");

  program.add_argument("-s", "--stylesheet")
      .default_value(std::string(
          "https://cdn.jsdelivr.net/gh/kimeiga/bahunya/dist/bahunya.min.css"))
      .help("The stylesheet file link");

  program.add_argument("-c", "--config").help("The config JSON file");

  if ((std::count(argv.begin(), argv.end(), "-c") == 0) &&
      (std::count(argv.begin(), argv.end(), "--config") == 0)) {
    program.add_argument("-i", "--input")
        .required()
        .help("The input file / directory");
  } else {
    program.add_argument("-i", "--input").help("The input file / directory");
  }

  try {
    program.parse_args(argv);
  } catch (const std::runtime_error& err) {
    std::cerr << program;
    throw err;
  }

  std::map<std::string, std::string> options;

  options["output"] = program.get("-o");
  options["stylesheet"] = program.get("-s");

  if (auto p_value = program.present("-c")) {
    std::ifstream json_stream(*p_value);
    if (json_stream.fail()) {
      throw std::runtime_error("Configuration file not found");
    }
    nlohmann::json config_json;
    json_stream >> config_json;

    if (config_json.find("input") == config_json.end()) {
      throw std::runtime_error(
          "Input file / directory not specified in config file");
    }

    if (auto input_file_optional = program.present("-i")) {
      options["input"] = *input_file_optional;
    }

    for (const auto& entry : config_json.items()) {
      options[entry.key()] = entry.value();
    }
  } else {
    options["input"] = program.get("-i");
  }

  return options;
}
