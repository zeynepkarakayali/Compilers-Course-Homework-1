
Here's how this is supposed to work


## Project Overview

- **Language**: C++
- **Build System**: CMake
- **Platform**: Linux (Working on other platforms is unsupported, but possible)
- **Entry Point**: `kirazc` (the kiraz compiler)

## Requirements

### Features

1. **Lexer**
   - The lexer must recognize all valid tokens in the Kiraz language.
   - Any unrecognized token must trigger an error with the line and character position.

2. **Parser**
   - The parser should construct an Abstract Syntax Tree (AST) from valid Kiraz code and produce meaningful error messages for invalid syntax.

3. **Modes of Operation**
   - `kirazc` should support two modes:
     - **File Mode**: Accept `.ki` files and print the corresponding parse tree (`-f` option).
     - **String Mode**: Accept Kiraz code strings and print the corresponding parse tree (`-s` option).

### Error Reporting

- Errors should be reported with the exact line number and character position.
- The project must gracefully handle invalid inputs and terminate with clear error messages.

### Lexical Elements

- **Alphabet**: The language includes:
  - 26 letters of the English alphabet (upper and lower case).
  - Arabic digits (0-9).
  - Special symbols: `{}`, `()`, `+`, `-`, `*`, `/`, `<`, `=`, `>`, `.`.
- **Identifiers**: Identifiers must start with a letter or underscore (`_`), followed by letters, digits, or underscores.
- **Integer Literals**: Composed of at least one digit.
- **String Literals**: Enclosed in double quotes (`"`).

### Keywords

- `import`
- `func`
- `if`
- `while`
- `class`

### Grammar and AST

The following constructs are supported in the parser:

1. **Function Definitions (`func`)**:
   - Composed of the keyword `func`, a function name, argument list, return type, and function body.
   - The argument list can have zero or more arguments, each with a type annotation.

2. **Statements**:
   - The **let statement**: Declares variables with optional initialization.
   - The **assignment statement**: Assigns values to variables.
   - **Arithmetic expressions**: Support basic operators (`+`, `-`, `*`, `/`).

## Project Structure

The project comes with a skeleton code including:

- **Token Class**: Represents terminal nodes.
- **Node Class**: Represents AST nodes.
- **Lexer and Parser**: To be implemented using Flex and Bison.

## Example Commands

### Compile the Project

```
mkdir build
cd build
cmake ../hw1 -DCMAKE_VERBOSE_MAKEFILE=1
make -j`nproc`

```

For Mac users installed flex w "brew":

```
mkdir build
cd build
cmake ../ -D FL_LIBRARY=/opt/homebrew/opt/flex/lib/libfl.a -D FL_INCLUDE_DIR=/opt/homebrew/opt/flex/include -D BISON_EXECUTABLE=/opt/homebrew/opt/bison/bin/bison
make -j`nproc`
```

Once the code compiles, you can use the ``ctest`` command to run the
test suite. Peruse ``ctest --help`` output for more info

Cheatsheet:

```shell
ctest -j`nproc`
ctest --rerun-failed
ctest --output-on-failure
ctest -V
ctest -R plus.*  # (regex)
```

For Mac users installed flex w "brew":

```
mkdir build
cd build
cmake ../ -D FL_LIBRARY=/opt/homebrew/opt/flex/lib/libfl.a -D FL_INCLUDE_DIR=/opt/homebrew/opt/flex/include -D BISON_EXECUTABLE=/opt/homebrew/opt/bison/bin/bison
make -j`nproc`
```

Once the code compiles, you can use the ``ctest`` command to run the
test suite. Peruse ``ctest --help`` output for more info

Cheatsheet:

```shell
ctest -j`nproc`
ctest --rerun-failed
ctest --output-on-failure
ctest -V
ctest -R plus.*  # (regex)
```
