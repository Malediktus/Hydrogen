# Hydrogen is a lightweight game engine written in C++17

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

Style guidelines:

- See .clang-format file
