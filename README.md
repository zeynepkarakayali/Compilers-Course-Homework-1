# Kiraz Compiler Project - Homework 1

This project implements a **lexer** and a **parser** for the "kiraz" programming language. The primary goal of this homework is to build a compiler that can parse and analyze `.ki` source files or raw code strings.

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

```bash
mkdir build
cd build
cmake ..
make

```

### Run the Compiler
To parse a `.ki` file and print its parse tree:

```bash
./kirazc -f <source_file>.ki
To parse a Kiraz code string and print its parse tree:
```

```bash
./kirazc -s "source_code_string"
```

### Error Examples
```bash
$ ./kirazc -s ’(’
** Parser Error at 1:1 at token: OP_LPAREN

$ ./kirazc -s ’1#2’ # not in alphabet of kiraz
** Parser Error at 1:2 at token: REJECTED(#)

$ ./kirazc -s ’1+2*3+4)’
** Parser Error at 1:8 at token: OP_RPAREN

$ ./kirazc -s ’1+2*3+4’ # no error
Sub(Add(Integer(1),Mult(Integer(2),Integer(3))),Integer(4))
```