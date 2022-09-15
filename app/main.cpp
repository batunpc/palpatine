#include <argparse/argparse.hpp>
#include <iostream>
#include <vector>
#include <fstream>

using std::string, std::vector, std::ifstream, std::ofstream, std::endl;
namespace fs = std::filesystem;

class Palpatine {
  public:
  // Static Site generator class with output & input path
  // and stylesheet link
  Palpatine(string output, string input, string stylesheet) {
    // paths to the input and output directories
    this->output = (fs::path(output) / "dist").string();
    this->input = input;
    // default stylesheet
    this->stylesheet = vector{string("./style.css")};
    // if a stylesheet is provided, add it to the stylesheet array
    if (stylesheet != "")
      this->stylesheet.push_back(stylesheet);
    // remove the output directory if it exists
    fs::remove_all(this->output);
    // create the output directory if it doesn't exist
    fs::create_directory(this->output);
    // create default stylesheet file
    this->create_stylesheet_file();
  }

  private:
  string input, output;
  vector<string> stylesheet;

  // Create StyleSheet file for generated HTML Files
  void create_stylesheet_file() {
    string css = R"(
      //TODO: Add CSS here
        )";

    ofstream css_out_file(fs::path(output) / "style.css");
    css_out_file << css << endl;
    css_out_file.close();
  }
};

argparse::ArgumentParser setup_parser() {
  argparse::ArgumentParser program("palpatine", "0.1");
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

int main(int argc, char const *argv[]) {
  auto program = setup_parser();

  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }

  // Create the generator object
  Palpatine static_site_gen(
      program.get("-o"),
      program.present("-i").value(),
      program.get("-s"));

  return 0;
}
