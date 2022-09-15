
#include <iostream>
#include <vector>
#pragma once

using std::string;

class Palpatine {
public:
  Palpatine(string, string, string);
  void generate();

private:
  string input, output;
  std::vector<string> stylesheet;

  void create_stylesheet_file();
  void process_path(string, string, string);
  void generate_page_file(string, string, std::vector<string>);
  void generate_index_file(string, string, std::vector<string>,
                           std::vector<string>);
};
