# Project Code File Structure

## Folder: ast
This folder includes three modules: stack, ast, and astmath.

### stack.c/stack.h

This module provides the implementation and interface for a stack data structure that stores pointers to `Node` elements. The stack is implemented using a dynamic array and supports basic stack operations such as push, pop, peek, and checking if the stack is empty.

### ast.c/ast.h

This module provides the implementation and interface for an Abstract Syntax Tree (AST) used to represent mathematical expressions. It includes definitions for node types, node structures, and functions to create, manipulate, and convert ASTs.

### astmath.c/astmath.h

This module provides mathematical operations that can be performed on the AST, such as differentiation, finding variables, and checking structural equality of nodes.
 
## Folder: simplificator
This folder includes two modules: one_layer and monomial.

### one_layer.c/one_layer.h

This module provides the implementation and interface for one-layer simplification of an AST. It simplifies mathematical expressions by applying basic algebraic rules.

Simplifies a node by applying one-layer simplification rules:
  - `1*node = node`
  - `0*node = 0`
  - `node+0 = node`
  - `node-0 = node`
  - `ln(1) = 0`
  - `pow(1, node) = 1`
  - `pow(node, 0) = 1`
  - `pow(node, 1) = node`
  - `0/node = 0`
  - `node/1 = node`
  - `0-node = -node`.

### monomial.c/monomial.h

This module provides the implementation and interface for handling monomials in the simplification process of mathematical expressions. It includes functions to create, manipulate, and convert monomials to and from AST nodes.


## Misc and executables

### utils.c/utils.h

This module provides utility functions that are used across various parts of the project. These functions include string manipulation, mathematical computations, and expression modifications.

### simplificator.c/simplifictor(.exe)

This module provides the main entry point for the simplification program. It reads a mathematical expression from the command line, processes it through various simplification steps, and outputs the simplified expression.

- **Usage**:
    - `simplificator <expression>`: Simplifies the given mathematical expression and prints the result.

### differentiate.c/differentiate(.exe)
This module provides the main entry point for the differentiation program. It reads a mathematical expression and a variable from the command line, processes the expression to compute its derivative with respect to the given variable, and outputs the result.

- **Usage**:
    - `differentiate <expression> [<variable>]`: Computes the derivative of the given mathematical expression with respect to the specified variable. If no variable is specified, it computes the derivative with respect to all variables found in the expression.

### differentiate_simplified.c/differentiate_simplified(.exe)

This module provides a simplified entry point for the differentiation program. It reads a mathematical expression and optionally a variable from the command line, processes the expression to compute its derivative with respect to the given variable, simplifies the derivative, and outputs the result.

- **Usage**:
    - `differentiate_simplified <expression> [<variable>]`: Computes the derivative of the given mathematical expression with respect to the specified variable. If no variable is specified, it computes the derivative with respect to all variables found in the expression and simplifies the result.

# How to Build and Run

> If you feel confused, you can skip to [Binary Release](#release)

This project is organized with CMake, a cross-platform build system. Below are the steps to build and run the project on your local machine.

## Prerequisites

- **CMake**
- **C Compiler**: A C compiler like `gcc` or `clang`.
- **ninja** or **make**

## Building the Project

1. **Open terminal in the code folder**：  
   Open terminal and change directory to the code folder

2. **Create a Build Directory**:  
   Create a separate directory for building the project to keep the source tree clean.

   ```bash
   mkdir build
   cd build
   ```

3. **Generate Build Files**:  
   Run CMake to generate the build files (e.g., Makefiles) in the build directory.

   ```bash
   cmake -G "Ninja" ..
   ```
   or
   ```bash
   cmake -G "Unix Makefiles" ..
   ```


   This command will read the `CMakeLists.txt` file in the parent directory and generate the necessary build files.

4. **Compile the Project**:  
   Use the generated build files to compile the project. 
   If you're using ninja, you can compile the project by running:

   ```bash
   ninja
   ```

   If you're using make, you can compile the project by running:

   ```bash
   make
   ```
   or (in Windows)
   ```bash
   mingw32-make
   ```
   

   This will compile the source code and generate the executables: `simplificator`, `differentiate`, and `differentiate_simplified`.  
   or (in Windows)  
   `simplificator.exe`, `differentiate.exe`, and `differentiate_simplified.exe`.

## Running the Executables

After successfully building the project, you can run the generated executables from the build directory.

1. **Run the Simplificator**:
   ```bash
   ./simplificator <expression>
   ```

2. **Run the Differentiate**:
   ```bash
   ./differentiate <expression> [<variable>]
   ```

3. **Run the Differentiate Simplified**:
   ```bash
   ./differentiate_simplified <expression> [<variable>]
   ```

## Cleaning the Build

If you want to clean the build and remove all the generated files, you can run:

```bash
ninja clean
```
or
```bash
make clean
```
or
```bash
mingw32-make clean
```

This will remove the compiled objects and executables, but keep the build files generated by CMake.

## Rebuilding the Project

If you make changes to the source code, you can rebuild the project by running:

`minja` or `make` or `ming32-make`

from the build directory. CMake will automatically detect changes and recompile only the necessary files.

## Additional CMake Options

- **Build Type**: You can specify the build type (e.g., Debug or Release) when generating the build files:

  ```bash
  cmake -DCMAKE_BUILD_TYPE=Debug ..
  ```

  or

  ```bash
  cmake -DCMAKE_BUILD_TYPE=Release ..
  ```

# <span id="release">Binary Release</span>

Binary releases for **Windows-amd64** and **Linux-amd64** are provided.

Remind that you shouldn't directly double-click the executable files, but run them in terminal with arguments quoted by "". 

## Windows 

**Compiler**: gcc version 14.2.0 (Rev2, Built by MSYS2 project)  
**Binaries**:
  - simplificator.exe :  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/uploads/2025/03/24/1228301267smL6ypHN.exe
  - differentiate.exe :
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/uploads/2025/03/24/1229168843J88qvybK.exe
  - differentiate_simplified.exe :
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/uploads/2025/03/24/1229347921kPQ9qLoz.exe

## Linux

