# C-Interpreter

A bytecode virtual machine interpreter implementation in C, designed as a learning project for understanding interpreter architecture and virtual machine concepts.

## Overview

This project implements a stack-based virtual machine that can execute bytecode instructions. The interpreter follows a traditional architecture with separate phases for scanning, compilation, and execution, similar to the Lox language implementation from "Crafting Interpreters".

## Architecture

The interpreter consists of several key components:

### Core Components

- **Scanner** (`scanner.c/h`) - Lexical analyzer that converts source code into tokens
- **Compiler** (`compiler.c/h`) - Currently implements token scanning and printing (parser implementation pending)
- **Virtual Machine** (`vm.c/h`) - Stack-based bytecode executor
- **Chunk** (`chunk.c/h`) - Bytecode container with constant pool and line information
- **Debug** (`debug.c/h`) - Disassembler for bytecode visualization
- **Value** (`value.c/h`) - Value system (currently supports double-precision numbers)
- **Memory** (`memory.c/h`) - Dynamic memory management utilities

### Supported Features

#### Tokens & Operators
- **Single-character tokens**: `()`, `{}`, `,`, `.`, `-`, `+`, `;`, `/`, `*`
- **One or two character tokens**: `!`, `!=`, `=`, `==`, `>`, `>=`, `<`, `<=`
- **Literals**: Numbers, Strings, Identifiers
- **Keywords**: `and`, `class`, `else`, `false`, `for`, `fun`, `if`, `nil`, `or`, `print`, `return`, `super`, `this`, `true`, `var`, `while`

#### Bytecode Instructions
- `OP_CONSTANT` - Load constant value
- `OP_CONSTANT_LONG` - Load constant with 24-bit index
- `OP_RETURN` - Return from function
- `OP_NEGATE` - Unary negation
- `OP_ADD`, `OP_SUBTRACT`, `OP_MULTIPLY`, `OP_DIVIDE` - Arithmetic operations

#### Virtual Machine Features
- **Stack-based execution** - 256-slot value stack
- **Constant pool** - Efficient storage for literal values
- **Line tracking** - Run-length encoded line information for debugging
- **Debug tracing** - Optional execution trace output

## Building

The project uses CMake for building:

```bash
mkdir build
cd build
cmake ..
make
```

Or use the existing build directory:
```bash
cd cmake-build-debug
cmake --build .
```

## Usage

### Web Demo
🌐 **[Try the live demo here!](https://software-engineering-courses-homeworks.github.io/C-Interpreter/)**

The web interface provides an easy way to test the interpreter without building from source:
- **Code Editor**: Write or paste your code with syntax highlighting
- **Drag & Drop**: Drop `.txt`, `.c`, or `.h` files directly into the editor
- **Dual Execution Modes**: 
  - **Run Code**: Execute in normal mode
  - **Run Debug**: Execute with debug output and tracing
- **Dark/Light Theme**: Toggle between themes with persistent settings
- **Real-time Output**: See execution results and error messages instantly

### Interactive Mode (REPL)
```bash
./C_Interpeter
```
Launches an interactive read-eval-print loop for testing expressions.

### File Execution
```bash
./C_Interpeter <filename>
```
Executes the specified source file.

### Current Limitations
- The compiler currently only performs lexical analysis (tokenization)
- No parser implementation yet - bytecode must be manually constructed
- Limited to arithmetic expressions with numeric values
- No variable declarations or control flow statements implemented

## Example

Currently, the interpreter can execute manually constructed bytecode:

```c
// Example from debug output:
0000    1 OP_CONSTANT         0 '1.2'
0002    | OP_CONSTANT         1 '3.4'
0004    | OP_ADD
0005    | OP_CONSTANT         2 '5.6'
0007    | OP_DIVIDE
0008    | OP_NEGATE
0009    | OP_RETURN
```

This would compute: `-(1.2 + 3.4) / 5.6`

## Memory Management

The interpreter includes a custom memory management system:
- Dynamic array growth with 2x capacity expansion
- Automatic memory cleanup on VM shutdown
- Run-length encoding for line information to minimize memory usage

## Debug Features

- **Bytecode disassembly** - Human-readable bytecode output
- **Execution tracing** - Real-time stack and instruction visualization
- **Line tracking** - Precise error location reporting

## Development Status

This is an educational project demonstrating:
- ✅ Lexical analysis (scanner)
- ✅ Bytecode virtual machine
- ✅ Stack-based execution
- ✅ Memory management
- ✅ Debug infrastructure
- ⏳ Parser implementation (in progress)
- ⏳ Code generation
- ⏳ Control flow
- ⏳ Variables and scoping

## Technical Details

- **Language**: C11
- **Build System**: CMake
- **Architecture**: Stack-based virtual machine
- **Value Type**: Double-precision floating point
- **Instruction Set**: Custom bytecode opcodes
- **Memory Model**: Dynamic arrays with automatic growth

## Educational Value

This project serves as an excellent introduction to:
- Interpreter design patterns
- Virtual machine architecture
- Bytecode execution models
- Memory management in systems programming
- Lexical analysis and tokenization
- Debug tool implementation

---

*A small project done between 2nd and 3rd years in college as part of software engineering coursework.*
