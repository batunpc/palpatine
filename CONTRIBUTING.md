# Contributing 

Thank you for your interest in contributing to palpatine! 🪅

> _"The Dark Side Of The Force Is A Pathway To Many Abilities Some Consider To Be Unnatural."_ - Palpatine

Discuss the change you wish to make via [issues page](https://github.com/batunpc/palpatine/issues)

### Environment setup

#### macOS
First install [Homebrew](https://brew.sh/) if you don't have it already. 
If you don't have Xcode installed, you can at least run the `xcode-select --install` command to install the command line tools. This will ensure you have (clang++, gcc, git etc.) installed.\
Test if you have the required tools with;
- `clang++ --version` , if not install by running `brew install clang++`
- `cmake --version` (tested with 3.15.4) - if not install with `brew install cmake`
- `make --version` - if not install with `brew install make`

#### Linux
Tested on Ubuntu 20.04.5 LTS (Focal Fossa) with;
- `clang++ --version` , if not install by running `sudo apt install clang++`
- `cmake --version` (tested with 3.15.4) - if not install with `sudo apt install cmake`
- `make --version` - if not install with `sudo apt install make`


### Building

Clone the repository and run either `./palpatine` or `make prepare` in the root directory of the project. 
> **Note**: If you get permission denied error when running `./palpatine` you can run `chmod +x palpatine` to give the executable permission to run. See more about chmod [here](https://www.howtogeek.com/437958/how-to-use-the-chmod-command-on-linux/).

This automates the traditional [cmake build process](https://cmake.org/cmake/help/latest/manual/cmake.1.html#generate-a-project-buildsystem) (i.e. `mkdir build && cd build && cmake .. && make`) and creates an executable file in the `build/app` directory. Preview the `prepare command` in [Makefile](https://github.com/batunpc/palpatine/blob/90dcb1c5898e44229b10c86253b458894855f6de/Makefile#L4) and executable [palpatine script](https://github.com/batunpc/palpatine/blob/main/palpatine) to see what commands are being run for better understanding.


### Dependencies
CMake is used to configure the following [dependencies as git submodules](https://github.com/batunpc/palpatine/blob/main/.gitmodules), and they will be installed in the `external` directory when the `Building` step is completed:
- [p-ranav/argparse](https://github.com/p-ranav/argparse) - A single-file header-only C++11 library for parsing command line arguments.
- [ikalnytskyi/termcolor](https://github.com/ikalnytskyi/termcolor) - A header-only C++ library for printing colored messages to a terminal.
- [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++. Used to parse the config file and generate the static site.

You can visualize these dependencies by running `make prepare` in your terminal and view the image file from `build/dependency.png`. This command will require `graphviz` and `dot` to be installed in your system.\
You can install this with `brew install graphviz && brew install dot`
> This step is completely optional and only useful to visualize what dependencies are required. Currently, it looks like this -
> 
<img width="1636" alt="CleanShot 2022-11-03 at 23 37 13@2x" src="https://user-images.githubusercontent.com/71259399/199881523-330b4a46-768a-497a-9687-9e1b16fdecc0.png">



## Raise PR

1. Update the README.md depending on the changes you made.
2. Increase the version numbers `MAJOR.MINOR.PATCH` that this change will be released as. 

