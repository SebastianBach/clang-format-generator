
# Clang Format Generator

[![build jobs](https://github.com/SebastianBach/clang-format-generator/actions/workflows/build.yml/badge.svg)](https://github.com/SebastianBach/clang-format-generator/actions/workflows/build.yml)

Command line tool that creates a ```clang-format``` file from a formatted C++ source code file.

See https://clang.llvm.org/docs/ClangFormat.html 


# Build

Requires a ```C++20``` compiler.

```sh
mkdir build
cd build
cmake  ..
cmake --build . --config Release
ctest -C Release  -VV
```

# Usage

## Create the Reference Source File

Use the tool to create a reference source file:

```sh
clang_format_generator source_file.cpp
```

## Format the Reference Source File

Format the generated source file as you like. Do not add or remove anything.

## Create the clang-format File

Run the tool to parse the reference file and create the clang-format file. Specify the target version of *clang-format*.

```sh
clang_format_generator source_file.cpp .clang-format 160
```

Define the target version of *clang-format* as an integer. For example, use ```160``` for version 16.0.

