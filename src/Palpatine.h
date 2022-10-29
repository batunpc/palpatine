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
  auto get_stylesheet() -> vector<string> { return stylesheets; }

private:
  string input, output;
  std::vector<string> stylesheets;
  void process_path(const string &, const string &, const string &);

  void generate_index_file(string, const string &, const vector<string> &,
                           const vector<string> &);
};
