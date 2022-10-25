
<h1 align="center">
  <br>
<img src="https://i.imgur.com/774fPlh.png" alt="Markdownify" width="200">
  <br>
  palpatine
</h1>

<h4 align="center">A minimal static site generator (SSG) built with C++ <br><br>
  <a href="https://emperor-palpatine.netlify.app/">Demo</a> .
  <a href="https://dev.to/batunpc/palpatine-release10-350g">Blog</a>

</h4>


## Overview 
Palpatine is a minimal static site generator (SSG) built with C++. It is a command line tool that takes a directory of raw data (.txt files and markdown) and generates a static website. It is designed to be fast, simple and easy to use.

### Requirements 
#### macOS
First install [Homebrew](https://brew.sh/) if you don't have it already. 
If you don't have Xcode installed, you can at least run the `xcode-select --install` command to install the command line tools. This will ensure you have (clang++, gcc, git etc.) installed.\
Test if you have the required tools with;
- `clang++ --version` , if not install by running `brew install clang++`
- `cmake --version` (tested with 3.15.4) - if not install with `brew install cmake`
- `make --version` - if not install with `brew install make`



### Usage
<!-- To clone and run this application, you'll need [Git](https://git-scm.com), C++ compiler with `std=c++17` supported and [CMake](https://cmake.org/) to build the project. Normally you would have to specify particular cmake commands to build the project but I have automated that process in the `Makefile` so all you have to do is run `make prepare` in the root directory of the project. This will build the project and create an executable file in the `build/app` directory. You can examine the the Makefile to see what it does. -->
Clone the repository and run either `./palpatine` or `make prepare` in the root directory of the project. 
This automates the traditional [cmake build process](https://cmake.org/cmake/help/latest/manual/cmake.1.html#generate-a-project-buildsystem) (i.e. `mkdir build && cd build && cmake .. && make`) and creates an executable file in the `build/app` directory. 


```bash
 git clone https://github.com/batunpc/palpatine && cd palpatine

make prepare 

./palpatine -i <input> -c <config>  #  -i OR -c flags are mandatory
```

> **Note**: If you get permission denied error when running `./palpatine` you can run `chmod +x palpatine` to give the executable permission to run. See more about chmod [here](https://www.howtogeek.com/437958/how-to-use-the-chmod-command-on-linux/).


### Flags
| Flag | Description | Required / Optional |
| --- | --- | --- |
| `-i` | Specify raw data directory or file e.g. use `data` directory in the codebase | Required <br> <if -c not specified> |
| `-c` | Specify config file e.g. use `config.json` in the codebase | Required<br> <if -i not specified> |
| `-o` | Specify particular directory that you want to generate static sites to. | Optional |
| `-s` | If you please, you can add custom stylesheets by specifying the css files.<br> By default it uses [bahunya](https://hakanalpay.com/bahunya/). Make sure the URL tail has extension `.css`| Optional|
| `-h` | This will display all the available flags that palpatine handles | Optional |


### Dependencies
CMake is used to confgure the following dependencies and they will be installed in the `external` directory:
- [p-ranav/argparse](https://github.com/p-ranav/argparse) - A single-file header-only C++11 library for parsing command line arguments.
- [ikalnytskyi/termcolor](https://github.com/ikalnytskyi/termcolor) - A header-only C++ library for printing colored messages to a terminal.
- [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++. Used to parse the config file and generate the static site.

## Features
 
- [x]  Generate a static site from a directory of text/markdown files
- [x]  Option to change the output directory
- [x]  Option to specify a config file
- [x]  Option to include a custom stylesheet link
- [x]  Generate a list of all pages in a directory, with links to each page See [inital page](https://emperor-palpatine.netlify.app/)
- [x]  Parse page title from the first line of the file if given
- [x]  Full markdown support
