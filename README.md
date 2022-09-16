
<h1 align="center">
  <br>
<img src="https://i.imgur.com/774fPlh.png" alt="Markdownify" width="200">
  <br>
  palpatine
  <br>
</h1>

<h4 align="center">A minimal static site generator (SSG) built with C++ </h4>


## Overview 
Palpatine is a minimal static site generator (SSG) built with C++. It is a command line tool that takes a directory of raw data (currently only txt) and generates a static website. It is designed to be fast, simple and easy to use.

### Requirements 
#### macOS
First install [Homebrew](https://brew.sh/) if you don't have it already. Then you can install the following dependencies:
- If you don't have Xcode installed, you can at least run the `xcode-select --install` command to install the command line tools. This will ensure you have (clang++, gcc, git etc.) installed.
- C++ compiler (tested with clang++) 
- CMake (tested with 3.15.4) - install with `brew install cmake`


### How To Use
To clone and run this application, you'll need [Git](https://git-scm.com), C++ compiler with `std=c++17` supported and [CMake](https://cmake.org/) to build the project.

```bash
# Clone this repository
$ git clone https://github.com/batunpc/palpatine

# Go into the repository
$ cd palpatine

# Build project w/ cmake and install dependencies 
$ make prepare

# Run the script
$ ./palpatine -i <input> -o <output> -s <stylesheet>
```
>**Note:** If you get permission denied error, you can run `chmod +x palpatine` to make the script executable.
### Flags
| Flag | Description | Required / Optional |
| --- | --- | --- |
| `-i` | Specify raw data directory or file e.g. use `data` directory in the codebase | Required |
| `-o` | Specify particular directory that you want to generate static sites to. By default `./dist` | Optional |
| `-s` | If you please, you can add custom stylesheets  by specifying the css files.<br> e.g  `-s https://unpkg.com/bamboo.css@1.3.9/dist/bamboo.min.css`| Optional|


### Dependencies
CMake is used to confgure the following dependencies:
- [p-ranav/argparse](https://github.com/p-ranav/argparse) - A single-file header-only C++11 library for parsing command line arguments.
- [ikalnytskyi/termcolor](https://github.com/ikalnytskyi/termcolor) - A header-only C++ library for printing colored messages to a terminal.

