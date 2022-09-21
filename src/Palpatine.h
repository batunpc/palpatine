
#include <iostream>
#include <vector>
#pragma once

using std::string, std::vector;

class Palpatine {
public:
  // Palpatine(string, string, string);
  Palpatine();
  Palpatine(const char *, const char *, const char *);
  void generate();

  vector<string> get_stylesheet() { return stylesheet; }
  void test() { std::cout << "test" << std::endl; }

private:
  string input, output;
  std::vector<string> stylesheet;

  void process_path(string, string, string);
  void generate_page_file(string, string, vector<string>);
  void generate_index_file(string, string, vector<string>, vector<string>);
};
