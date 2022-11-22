#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do
                           // this in one cpp file
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "FileHandler.h"
#include "Palpatine.cpp"
#include "Palpatine.h"
#include "parser.h"
#include "htmlplus.h"

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
      config << R"({"input":"./data"})";
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
      config << R"({"input":"./in", "output":"./output", "featureX":"abcd"})";
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

TEST_CASE("Text File to Static HTML", "[txt]") {
  std::vector<std::string> vct{"test-css"};
  TextHandler handler(vct);

  SECTION("Text File without Title") {
    fs::path file_path = fs::temp_directory_path() / "Test Text.txt";
    fs::path out_path = fs::temp_directory_path();
    std::string name = "Test Text HTML.html";
    {
      std::ofstream textfile(file_path);
      textfile << "This is a paragraph.\nThe paragraph is contionued. It is "
                  "going on until two new-line.\n\n";
      textfile << "This is another paragraph.";
    }

    handler.process(file_path.string(), out_path.string(), name);
    // std::cout << "Out Path: " << (out_path / name).string() << std::endl;
    std::string output;
    {
      std::ifstream out_file(out_path / name);
      std::stringstream ss;
      ss << out_file.rdbuf();
      output = ss.str();
    }

    // CSS
    REQUIRE(output.find(R"(<link rel="stylesheet" href="test-css">)") !=
            std::string::npos);

    // Title
    REQUIRE(output.find("<title>Test Text</title>") != std::string::npos);
    REQUIRE(
        output.find(
            R"(<h1 class="animate__animated animate__fadeInLeft">Test Text</h1>)") !=
        std::string::npos);

    // Paragraphs
    REQUIRE(output.find(R"(<p>This is a paragraph.
The paragraph is contionued. It is going on until two new-line.</p>)") !=
            std::string::npos);
    REQUIRE(output.find(R"(<p>This is another paragraph.</p>)") !=
            std::string::npos);
  }

  SECTION("Text File with File Title") {
    fs::path file_path = fs::temp_directory_path() / "Test Text.txt";
    fs::path out_path = fs::temp_directory_path();
    std::string name = "Test Text HTML.html";
    {
      std::ofstream textfile(file_path);
      textfile << "This is title\n\n\n";  // 2 blank line
      textfile << "This is a paragraph.\nThe paragraph is contionued. It is "
                  "going on until two new-line.\n\n";
      textfile << "This is another paragraph.";
    }

    handler.process(file_path.string(), out_path.string(), name);
    // std::cout << "Out Path: " << (out_path / name).string() << std::endl;
    std::string output;
    {
      std::ifstream out_file(out_path / name);
      std::stringstream ss;
      ss << out_file.rdbuf();
      output = ss.str();
    }

    // CSS
    REQUIRE(output.find(R"(<link rel="stylesheet" href="test-css">)") !=
            std::string::npos);

    // Title
    REQUIRE(output.find("<title>This is title</title>") != std::string::npos);
    REQUIRE(
        output.find(
            R"(<h1 class="animate__animated animate__fadeInLeft">This is title</h1>)") !=
        std::string::npos);

    // Paragraphs
    REQUIRE(output.find(R"(<p>This is a paragraph.
The paragraph is contionued. It is going on until two new-line.</p>)") !=
            std::string::npos);
    REQUIRE(output.find(R"(<p>This is another paragraph.</p>)") !=
            std::string::npos);
  }
}

TEST_CASE("Markdown to HTML", "[md]") {
  std::vector<std::string> vct{"test-css-file"};
  MarkdownHandler handler(vct);

  SECTION("Markdown File Formatting") {
    fs::path file_path = fs::temp_directory_path() / "Test Markdown.md";
    fs::path out_path = fs::temp_directory_path();
    std::string name = "Test Markdown.html";
    {
      std::ofstream mdfile(file_path);
      mdfile << "# This is heading 1\nThis is a paragraph with __bold__ and "
                "_italic_ text with [link](https://www.google.com) and image "
                "![](https://www.google.com)\n\n";
      mdfile << "## This is heading 2\n";
      mdfile << "### This is heading 3\n";
      mdfile << "---\n";  // hr
      mdfile << "This is another paragarph\n";
    }

    handler.process(file_path.string(), out_path.string(), name);
    // std::cout << "Out Path: " << (out_path / name).string() << std::endl;
    std::string output;
    {
      std::ifstream out_file(out_path / name);
      std::stringstream ss;
      ss << out_file.rdbuf();
      output = ss.str();
    }

    // CSS
    REQUIRE(output.find(R"(<link rel="stylesheet" href="test-css-file">)") !=
            std::string::npos);

    // Title
    REQUIRE(output.find("<title>Test Markdown</title>") != std::string::npos);
    REQUIRE(
        output.find(
            R"(<h1 class="animate__animated animate__fadeInLeft">Test Markdown</h1>)") !=
        std::string::npos);

    // Heading
    REQUIRE(output.find(R"(<h1>This is heading 1</h1>)") != std::string::npos);
    REQUIRE(output.find(R"(<h2>This is heading 2</h2>)") != std::string::npos);
    REQUIRE(output.find(R"(<h3>This is heading 3</h3>)") != std::string::npos);

    // HR
    REQUIRE(output.find(R"(<hr/>)") != std::string::npos);

    // Paragraph
    REQUIRE(
        output.find(
            R"(<p>This is a paragraph with <strong>bold</strong> and <em>italic</em> text with <a href="https://www.google.com">link</a> and image <img src="https://www.google.com" alt=""/> </p>)") !=
        std::string::npos);
    REQUIRE(output.find(R"(<p>This is another paragarph </p>)") !=
            std::string::npos);
  }
}

TEST_CASE("Palpatine Tests", "[palpatine]") {
  fs::create_directory(fs::temp_directory_path() / "out");

  SECTION("Single File") {
    Palpatine palpatine(
        (fs::temp_directory_path() / "out").string().c_str(),
        (fs::temp_directory_path() / "input.txt").string().c_str(),
        (const char *)"test-style-sheet-link");
    fs::path file_path = fs::temp_directory_path() / "input.txt";
    fs::path out_path = fs::temp_directory_path() / "out";
    std::string name = "index.html";
    {
      std::ofstream textfile(file_path);
      textfile << "This is title\n\n\n";  // 2 blank line
      textfile << "This is a paragraph.\nThe paragraph is contionued. It is "
                  "going on until two new-line.\n\n";
      textfile << "This is another paragraph.";
    }

    palpatine.generate();
    std::string output;
    {
      std::ifstream out_file(out_path / "dist" / name);
      std::stringstream ss;
      ss << out_file.rdbuf();
      output = ss.str();
    }

    // CSS
    REQUIRE(output.find(
                R"(<link rel="stylesheet" href="test-style-sheet-link">)") !=
            std::string::npos);

    // Title
    REQUIRE(output.find("<title>This is title</title>") != std::string::npos);
    REQUIRE(
        output.find(
            R"(<h1 class="animate__animated animate__fadeInLeft">This is title</h1>)") !=
        std::string::npos);

    // Paragraphs
    REQUIRE(output.find(R"(<p>This is a paragraph.
The paragraph is contionued. It is going on until two new-line.</p>)") !=
            std::string::npos);
    REQUIRE(output.find(R"(<p>This is another paragraph.</p>)") !=
            std::string::npos);
  }

  SECTION("SINGLE FOLDER") {
    Palpatine palpatine((fs::temp_directory_path() / "out").string().c_str(),
                        (fs::temp_directory_path() / "input").string().c_str(),
                        (const char *)"test-style-sheet-link");
    fs::create_directory(fs::temp_directory_path() / "input");

    auto txt_path = fs::temp_directory_path() / "input" / "Text File.txt";
    auto md_path = fs::temp_directory_path() / "input" / "MD File.md";

    {
      std::ofstream textfile(txt_path);
      textfile << "This is title\n\n\n";  // 2 blank line
      textfile << "This is a paragraph.\nThe paragraph is contionued. It is "
                  "going on until two new-line.\n\n";
      textfile << "This is another paragraph.";
    }

    {
      std::ofstream mdfile(md_path);
      mdfile << "# This is heading 1\nThis is a paragraph with __bold__ and "
                "_italic_ text with [link](https://www.google.com) and image "
                "![](https://www.google.com)\n\n";
      mdfile << "## This is heading 2\n";
      mdfile << "### This is heading 3\n";
      mdfile << "---\n";  // hr
      mdfile << "This is another paragarph\n";
    }

    palpatine.generate();

    SECTION("Index File") {
      std::string output;
      std::ifstream out_file(fs::temp_directory_path() / "out" / "dist" /
                             "index.html");
      std::stringstream ss;
      ss << out_file.rdbuf();
      output = ss.str();

      // CSS
      REQUIRE(output.find(
                  R"(<link rel="stylesheet" href="test-style-sheet-link">)") !=
              std::string::npos);

      // Title
      REQUIRE(output.find("<title>Homepage</title>") != std::string::npos);

      // List
      REQUIRE(
          output.find(
              R"(<li><a href="Text File.html"><i class="fa-regular fa-file-lines"></i> Text File</a></li>)") !=
          std::string::npos);
      REQUIRE(
          output.find(
              R"(<li><a href="MD File.html"><i class="fa-regular fa-file-lines"></i> MD File</a></li>)") !=
          std::string::npos);
    }

    SECTION("Text File") {
      std::string output;
      std::ifstream out_file(fs::temp_directory_path() / "out" / "dist" /
                             "Text File.html");
      std::stringstream ss;
      ss << out_file.rdbuf();
      output = ss.str();

      // CSS
      REQUIRE(output.find(
                  R"(<link rel="stylesheet" href="test-style-sheet-link">)") !=
              std::string::npos);

      // Title
      REQUIRE(output.find("<title>This is title</title>") != std::string::npos);
      REQUIRE(
          output.find(
              R"(<h1 class="animate__animated animate__fadeInLeft">This is title</h1>)") !=
          std::string::npos);

      // Paragraphs
      REQUIRE(output.find(R"(<p>This is a paragraph.
The paragraph is contionued. It is going on until two new-line.</p>)") !=
              std::string::npos);
      REQUIRE(output.find(R"(<p>This is another paragraph.</p>)") !=
              std::string::npos);
    }

    SECTION("Markdown File") {
      std::string output;
      std::ifstream out_file(fs::temp_directory_path() / "out" / "dist" /
                             "MD File.html");
      std::stringstream ss;
      ss << out_file.rdbuf();
      output = ss.str();

      // CSS
      REQUIRE(output.find(
                  R"(<link rel="stylesheet" href="test-style-sheet-link">)") !=
              std::string::npos);

      // Title
      REQUIRE(output.find("<title>MD File</title>") != std::string::npos);
      REQUIRE(
          output.find(
              R"(<h1 class="animate__animated animate__fadeInLeft">MD File</h1>)") !=
          std::string::npos);

      // Heading
      REQUIRE(output.find(R"(<h1>This is heading 1</h1>)") !=
              std::string::npos);
      REQUIRE(output.find(R"(<h2>This is heading 2</h2>)") !=
              std::string::npos);
      REQUIRE(output.find(R"(<h3>This is heading 3</h3>)") !=
              std::string::npos);

      // HR
      REQUIRE(output.find(R"(<hr/>)") != std::string::npos);

      // Paragraph
      REQUIRE(
          output.find(
              R"(<p>This is a paragraph with <strong>bold</strong> and <em>italic</em> text with <a href="https://www.google.com">link</a> and image <img src="https://www.google.com" alt=""/> </p>)") !=
          std::string::npos);
      REQUIRE(output.find(R"(<p>This is another paragarph </p>)") !=
              std::string::npos);
    }

    fs::remove_all(fs::temp_directory_path() / "input");
    fs::remove_all(fs::temp_directory_path() / "out");
  }

  SECTION("Nested FOLDER") {
    Palpatine palpatine((fs::temp_directory_path() / "out").string().c_str(),
                        (fs::temp_directory_path() / "input").string().c_str(),
                        (const char *)"test-style-sheet-link");
    fs::create_directories(fs::temp_directory_path() / "input" / "folder");

    auto txt_path =
        fs::temp_directory_path() / "input" / "folder" / "Text File.txt";
    auto md_path =
        fs::temp_directory_path() / "input" / "folder" / "MD File.md";

    {
      std::ofstream textfile(txt_path);
      textfile << "This is title\n\n\n";  // 2 blank line
      textfile << "This is a paragraph.\nThe paragraph is contionued. It is "
                  "going on until two new-line.\n\n";
      textfile << "This is another paragraph.";
    }

    {
      std::ofstream mdfile(md_path);

      mdfile << "# This is heading 1\nThis is a paragraph with __bold__ and "
                "_italic_ text with [link](https://www.google.com) and image ![](https://www.google.com)\n\n";
      mdfile << "## This is heading 2\n";
      mdfile << "### This is heading 3\n";
      mdfile << "---\n";  // hr
      mdfile << "This is another paragarph\n";
    }

    palpatine.generate();

    SECTION("Index File") {
      std::string output;
      std::ifstream out_file(fs::temp_directory_path() / "out" / "dist" /
                             "index.html");
      std::stringstream ss;
      ss << out_file.rdbuf();
      output = ss.str();

      // CSS
      REQUIRE(output.find(
                  R"(<link rel="stylesheet" href="test-style-sheet-link">)") !=
              std::string::npos);

      // Title
      REQUIRE(output.find("<title>Homepage</title>") != std::string::npos);

      // List
      REQUIRE(
          output.find(
              R"(<li><a href="folder"/><i class="fas fa-folder"></i> folder</a></li>)") !=
          std::string::npos);
    }

    SECTION("Nested Folder Index File") {
      std::string output;
      std::ifstream out_file(fs::temp_directory_path() / "out" / "dist" /
                             "folder" / "index.html");
      std::stringstream ss;
      ss << out_file.rdbuf();
      output = ss.str();

      // CSS
      REQUIRE(output.find(
                  R"(<link rel="stylesheet" href="test-style-sheet-link">)") !=
              std::string::npos);

      // Title
      REQUIRE(output.find("<title>folder</title>") != std::string::npos);

      // List
      REQUIRE(
          output.find(
              R"(<li><a href="Text File.html"><i class="fa-regular fa-file-lines"></i> Text File</a></li>)") !=
          std::string::npos);
      REQUIRE(
          output.find(
              R"(<li><a href="MD File.html"><i class="fa-regular fa-file-lines"></i> MD File</a></li>)") !=
          std::string::npos);
    }

    SECTION("Text File") {
      std::string output;
      std::ifstream out_file(fs::temp_directory_path() / "out" / "dist" /
                             "folder" / "Text File.html");
      std::stringstream ss;
      ss << out_file.rdbuf();
      output = ss.str();

      // CSS
      REQUIRE(output.find(
                  R"(<link rel="stylesheet" href="test-style-sheet-link">)") !=
              std::string::npos);

      // Title
      REQUIRE(output.find("<title>This is title</title>") != std::string::npos);
      REQUIRE(
          output.find(
              R"(<h1 class="animate__animated animate__fadeInLeft">This is title</h1>)") !=
          std::string::npos);

      // Paragraphs
      REQUIRE(output.find(R"(<p>This is a paragraph.
The paragraph is contionued. It is going on until two new-line.</p>)") !=
              std::string::npos);
      REQUIRE(output.find(R"(<p>This is another paragraph.</p>)") !=
              std::string::npos);
    }

    SECTION("Markdown File") {
      std::string output;
      std::ifstream out_file(fs::temp_directory_path() / "out" / "dist" /
                             "folder" / "MD File.html");
      std::stringstream ss;
      ss << out_file.rdbuf();
      output = ss.str();

      // CSS
      REQUIRE(output.find(
                  R"(<link rel="stylesheet" href="test-style-sheet-link">)") !=
              std::string::npos);

      // Title
      REQUIRE(output.find("<title>MD File</title>") != std::string::npos);
      REQUIRE(
          output.find(
              R"(<h1 class="animate__animated animate__fadeInLeft">MD File</h1>)") !=
          std::string::npos);

      // Heading
      REQUIRE(output.find(R"(<h1>This is heading 1</h1>)") !=
              std::string::npos);
      REQUIRE(output.find(R"(<h2>This is heading 2</h2>)") !=
              std::string::npos);
      REQUIRE(output.find(R"(<h3>This is heading 3</h3>)") !=
              std::string::npos);

      // HR
      REQUIRE(output.find(R"(<hr/>)") != std::string::npos);

      // Paragraph
      REQUIRE(
          output.find(
              R"(<p>This is a paragraph with <strong>bold</strong> and <em>italic</em> text with <a href="https://www.google.com">link</a> and image <img src="https://www.google.com" alt=""/> </p>)") !=
          std::string::npos);
      REQUIRE(output.find(R"(<p>This is another paragarph </p>)") !=
              std::string::npos);
    }

    fs::remove_all(fs::temp_directory_path() / "input");
    fs::remove_all(fs::temp_directory_path() / "out");
  }
}

TEST_CASE("HTMLPlus Tests", "[html_plus]") {
  SECTION("Header") {
    std::stringstream html;
    HMTLPLUS::header(html, "Title of File", std::vector<std::string>{"test-css"});
    std::string content = html.str();
    std::string expected = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Title of File</title>

    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/animate.css/4.1.1/animate.min.css">
    <script src="https://kit.fontawesome.com/7c3efb728f.js" crossorigin="anonymous"></script>
    <style> ul { list-style-type: none; margin: 0; padding: 0; overflow: hidden;} </style>
    
    <link rel="stylesheet" href="test-css">
</head>
<body>
    <div>
        <h1 class="animate__animated animate__fadeInLeft">Title of File</h1>
    </div>)";
    REQUIRE(content.size() == expected.size());
  }

  SECTION("Footer") {
    std::stringstream html;
    HMTLPLUS::footer(html);
    std::string content = html.str();
    std::string expected = R"(
  </body>
</html>)";
    REQUIRE(content.size() == expected.size());
  }
}