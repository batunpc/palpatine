
#include <iostream>
#include <vector>
#pragma once

using std::string, std::vector;

class Palpatine {
public:
  // Palpatine(string, string, string);
  Palpatine(const char *, const char *, const char *);
  void generate();
  static std::vector<string> stylesheet;

private:
  string input, output;

  void process_path(string, string, string);
  void generate_page_file(string, string, vector<string>);
  void generate_index_file(string, string, vector<string>, vector<string>);
};
