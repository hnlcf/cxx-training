# Crimson

A general web server inspired by [nginx](https://github.com/nginx/nginx).

## Contents

- [Introduction](#introduction)
- [Installation](#installation)
- [Build](#build)
- [License](#license)

## Introduction

The inspiration for this project is attributed to nginx.

Learn and understand nginx makes me germinate the idea of writing such a library and put it into practice.

At present, I am still thinking about what to do, so let's start in the direction of nginx first!

## Installation

```shell
$ git clone https://github.com/hnlcf/crimson.git

$ cd crimson
```

## Build

### Using script

```shell
python build.py --all
```

### Manual

1. CMake config

```shell
$ cmake -DCMAKE_BUILD_TYPE=release -S . -B build
```

2. Build

```shell
$ cmake --build build --parallel 12
```

3. Run test

```
$ cd build && ctest -C --verbose
```

## License

MIT
