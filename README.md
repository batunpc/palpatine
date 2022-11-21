<a name="readme-top"></a>

<h1 align="center">
  <br>
<img src="https://i.imgur.com/774fPlh.png" alt="Markdownify" width="200">
  <br>
  palpatine
</h1>

<h4 align="center">

A minimal static site generator (SSG) built with C++ <br><br>
  <a href="https://emperor-palpatine.netlify.app/">Demo</a> .
  <a href="https://dev.to/batunpc/palpatine-release10-350g">Blog</a>
</h4>



## Overview 

Palpatine is a command line tool and a static site generator (SSG) that takes a directory of raw data (text and markdown) and generates a static website. It is designed to be fast, simple and easy to use. Find out how to install palpatine by reading [CONTRIBUTING.md](https://github.com/batunpc/palpatine/blob/main/CONTRIBUTING.md)

### Built with

![image](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white) 
![image](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)


### Available flags
| Flag | Description                                                                                                                                                                               | Required / Optional                 |
|------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------|
| `-i` | Specify raw data directory or file e.g. use `data` directory in the codebase                                                                                                              | Required <br> <if -c not specified> |
| `-c` | Specify config file e.g. use `config.json` in the codebase                                                                                                                                | Required<br> <if -i not specified>  |
| `-o` | Specify particular directory that you want to generate static sites to.                                                                                                                   | Optional                            |
| `-s` | Add custom stylesheets by specifying the css files.<br> By default it uses [bahunya](https://hakanalpay.com/bahunya/). | Optional                            |
| `-h` | This will display all the available flags that palpatine handles                                                                                                                          | Optional                            |


### Usage

> **Note**: The example commands below are using the `sample data` directory in the codebase. You can use your own data directory or config file.


#### Input

The most basic usage of palpatine is to specify the input directory (i.e. the directory that contains the raw data)


```bash
./palpatine -i data
```

#### Input & Output 

By default this will generate a `dist directory` within the project. You can specify a different output directory by using the `-o` flag


```bash
./palpatine -i data -o ~/Desktop
```

The above command will generate a `dist` directory in your Desktop.

#### Stylesheets 

Add a custom stylesheet to present your static site in a preffered way
(i.e. use [water.css](https://watercss.kognise.dev/)) Notice that the URL tail has extension `.css`

```bash
./palpatine -i data -s https://cdn.jsdelivr.net/npm/water.css@2/out/water.css
```

#### Config file

You can use a config file in JSON instead of specifying the each flag manually from CLI. The config file should be in the following format:

```json
{
  "input": "data",
  "output": "~/Desktop",
  "stylesheet": "https://cdn.jsdelivr.net/npm/water.css@2/out/water.css"
}
```

Then you can use the config file by specifying the `-c` flag
  
  ```bash
  ./palpatine -c config.json
  ```


### License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>
