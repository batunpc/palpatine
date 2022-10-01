#include "File.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
namespace file_sdds {
std::string read_file(std::string file_name) {
  std::string file_str;
  std::ifstream file_data(file_name);
  std::stringstream str_stream;
  str_stream << file_data.rdbuf();
  file_str = str_stream.str();
  file_data.close();
  return file_str;
}
} // namespace file_sdds
