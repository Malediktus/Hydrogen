# Vortex Renderer is a cross platform renderer written in C++ and supporting multiple graphics API

![Build Status](https://github.com/Malediktus/VortexRenderer/workflows/build/badge.svg) [![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

The requirements are:

- CMake 3.15 or better; 3.14+ highly recommended.
- A C++17 compatible compiler
- Git (highly recommend)
- Doxygen (optional)

To configure:

```bash
cmake -S . -B build
```

Add `-G{Generator}` where {Generator} is your desired generator.

To build:

```bash
cmake --build build
```

To build docs (requires Doxygen, output in `build/docs/html`):

```bash
cmake --build build --target docs
```

To use an IDE, such as Xcode or Visual Studio (where {IDE} is your desired IDE):

```bash
cmake -S . -B build -G{IDE}
cmake --open build
```

To use cpplint and/or include-what-you-need

```bash
cmake -DCMAKE_CXX_CPPCHECK:FILEPATH=cppcheck -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=include-what-you-use -S . -B build
cmake --open build
```

Style guidelines:

- See .clang-format file
