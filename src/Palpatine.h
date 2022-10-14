#include <iostream>
#include <string>
#include <vector>
#pragma once

using std::string, std::vector;

class Palpatine {
public:
  // Palpatine(string, string, string);
  Palpatine();
  Palpatine(const char *, const char *, const char *);
  void generate();
  vector<string> get_stylesheet() { return stylesheets; }

private:
  string input, output;
  std::vector<string> stylesheets;
  void process_path(string, string, string);

  void generate_index_file(string, string, vector<string>, vector<string>);
};
