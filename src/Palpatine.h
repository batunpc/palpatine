#include <iostream>
#include <string>
#include <vector>
#pragma once

using std::string, std::vector;

class Palpatine {
public:
  Palpatine(const char*, const char*, const char*);
  void generate();

private:
  string m_input, m_output;

  std::vector<string> stylesheets;

  void process_path(const string&, const string&, const string&);
  void generate_index_file(
      const string&, const string&, const vector<string>&,
      const vector<string>&);
};
