#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do
                           // this in one cpp file
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <string>
#include <vector>

#include "parser.h"

namespace fs = std::filesystem;

TEST_CASE("CLI Parser works perfectly", "[parser]") {
  std::vector<std::string> vct{"./palpatine"};

  SECTION("Without any args") {
    REQUIRE_THROWS_AS(get_options(vct), std::runtime_error);
  }

  SECTION("With only input args") {
    vct.emplace_back("--input");
    vct.emplace_back("./data");

    auto options = get_options(vct);

    REQUIRE(options["output"] == ".");
    REQUIRE(options["input"] == "./data");
    REQUIRE(options["stylesheet"] ==
            "https://cdn.jsdelivr.net/gh/kimeiga/bahunya/dist/bahunya.min.css");
  }

  SECTION("Input only default arguments") {
    vct.emplace_back("-i");
    vct.emplace_back("./input");

    auto options = get_options(vct);

    REQUIRE(options["output"] == ".");
    REQUIRE(options["input"] == "./input");
    REQUIRE(options["stylesheet"] ==
            "https://cdn.jsdelivr.net/gh/kimeiga/bahunya/dist/bahunya.min.css");
  }

  SECTION("Config File not found") {
    vct.emplace_back("-c");
    vct.emplace_back("config_none");

    REQUIRE_THROWS_AS(get_options(vct), std::runtime_error);
  }

  SECTION("Config File without input") {
    fs::path config_path = fs::temp_directory_path() / "config.json";
    {
      std::ofstream config(config_path.string());
      config << "{}";
    }
    vct.emplace_back("-c");
    vct.emplace_back(config_path.string());
    REQUIRE_THROWS_AS(get_options(vct), std::runtime_error);
  }

  SECTION("Config File with input") {
    fs::path config_path = fs::temp_directory_path() / "config.json";
    {
      std::ofstream config(config_path.string());
      config << "{\"input\":\"./data\"}";
    }
    vct.emplace_back("-c");
    vct.emplace_back(config_path.string());

    auto options = get_options(vct);

    REQUIRE(options["output"] == ".");
    REQUIRE(options["input"] == "./data");
    REQUIRE(options["stylesheet"] ==
            "https://cdn.jsdelivr.net/gh/kimeiga/bahunya/dist/bahunya.min.css");
  }

  SECTION("Config File with extra config") {
    fs::path config_path = fs::temp_directory_path() / "config.json";
    {
      std::ofstream config(config_path.string());
      config << "{\"input\":\"./in\", \"output\":\"./output\", "
                "\"featureX\":\"abcd\"}";
    }
    vct.emplace_back("-c");
    vct.emplace_back(config_path.string());

    auto options = get_options(vct);

    REQUIRE(options["output"] == "./output");
    REQUIRE(options["input"] == "./in");
    REQUIRE(options["stylesheet"] ==
            "https://cdn.jsdelivr.net/gh/kimeiga/bahunya/dist/bahunya.min.css");
  }
}

TEST_CASE("Config File Parsed Successfully", "[json]") {}
