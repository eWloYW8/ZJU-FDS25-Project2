[中文版本](#chinese)（机翻）附于末尾
---
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

### simplificator.c/simplifictor(.exe) <span id="executable-info"></span>

This module provides the main entry point for the simplification program. It reads a mathematical expression from the command line, processes it through various simplification steps, and outputs the simplified expression.

**Usage**:
   - `simplificator <expression>`: Simplifies the given mathematical expression and prints the result.

### differentiate.c/differentiate(.exe)
This module provides the main entry point for the differentiation program. It reads a mathematical expression and a variable from the command line, processes the expression to compute its derivative with respect to the given variable, and outputs the result.

**Usage**:
   - `differentiate <expression> [<variable>]`: Computes the derivative of the given mathematical expression with respect to the specified variable. If no variable is specified, it computes the derivative with respect to all variables found in the expression.

### differentiate_simplified.c/differentiate_simplified(.exe)

This module provides a simplified entry point for the differentiation program. It reads a mathematical expression and optionally a variable from the command line, processes the expression to compute its derivative with respect to the given variable, simplifies the derivative, and outputs the result.

**Usage**:
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

The function of the three files are mentioned in the first section, [see above](#executable-info).

Remind that you shouldn't directly double-click the executable files, but run them in terminal with arguments quoted by "". 

You can rename the downloaded file by yourself.

## Windows 

**Compiler**: gcc version 14.2.0 (Rev2, Built by MSYS2 project)  
**Binaries**:
  - simplificator.exe：  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/uploads/2025/03/25/194735821579BopmsS.exe
  - differentiate.exe：  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/uploads/2025/03/25/1946518359xmXdGs08.exe
  - differentiate_simplified.exe：  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/uploads/2025/03/25/1947186110vu1XsdYF.exe

    

## Linux

**Compiler**: gcc version 12.2.0 (Debian 12.2.0-14)  
**Binaries**:
  - simplificator：  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/simplificator_linux
  - differentiate：  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/differentiate_linux
  - differentiate_simplified：  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/differentiate_simplified_linux
    

---
---
# <span id="chinese">项目代码文件结构</span>

## 文件夹：ast
该文件夹包含三个模块：stack（栈）、ast（抽象语法树）和astmath（AST数学运算）。

### stack.c/stack.h

该模块提供了用于存储指向`Node`元素指针的栈数据结构的实现和接口。栈通过动态数组实现，支持基本的栈操作，如压栈（push）、弹栈（pop）、查看栈顶（peek）以及判断栈是否为空。

### ast.c/ast.h

该模块提供了用于表示数学表达式的抽象语法树（AST）的实现和接口。包含节点类型定义、节点结构以及用于创建、操作和转换AST的函数。

### astmath.c/astmath.h

该模块提供了对AST执行数学运算的功能，如求导、查找变量以及检查节点的结构等价性。

## 文件夹：simplificator
该文件夹包含两个模块：one_layer（单层简化）和monomial（单项式处理）。

### one_layer.c/one_layer.h

该模块提供了对AST进行单层简化的实现和接口。通过应用基本代数规则简化数学表达式。

支持的简化规则包括：
  - `1*节点 = 节点`
  - `0*节点 = 0`
  - `节点+0 = 节点`
  - `节点-0 = 节点`
  - `ln(1) = 0`
  - `pow(1, 节点) = 1`
  - `pow(节点, 0) = 1`
  - `pow(节点, 1) = 节点`
  - `0/节点 = 0`
  - `节点/1 = 节点`
  - `0-节点 = -节点`

### monomial.c/monomial.h

该模块提供了在数学表达式简化过程中处理单项式的实现和接口。包含创建、操作单项式以及与AST节点相互转换的功能。

## 杂项和可执行文件

### utils.c/utils.h

该模块提供项目各部分的通用工具函数，包括字符串操作、数学计算和表达式修改等功能。

### simplificator.c/simplificator(.exe) <span id="executable-info-chinese"></span>

该模块是表达式简化程序的主入口。从命令行读取数学表达式，经过多步简化处理后输出简化结果。

**使用方法**：
   - `simplificator <表达式>`：简化给定的数学表达式并打印结果。

### differentiate.c/differentiate(.exe)

该模块是求导程序的主入口。从命令行读取数学表达式和变量，计算表达式对指定变量的导数并输出结果。

**使用方法**：
   - `differentiate <表达式> [<变量>]`：计算表达式对指定变量的导数。若未指定变量，则对表达式中所有变量求导。

### differentiate_simplified.c/differentiate_simplified(.exe)

该模块是简化版求导程序的主入口。读取数学表达式和可选变量，计算导数后对结果进行简化并输出。

**使用方法**：
   - `differentiate_simplified <表达式> [<变量>]`：计算表达式对指定变量的导数并简化结果。若未指定变量，则对表达式中所有变量求导后简化。

# 构建与运行指南

> 若感到困惑，可直接跳至[二进制发布版](#release-chinese)

本项目使用跨平台构建系统CMake组织。以下是在本地机器上构建和运行的步骤。

## 前置条件

- **CMake**
- **C编译器**：如`gcc`或`clang`
- **ninja**或**make**

## 构建项目

1. **在代码文件夹中打开终端**：  
   打开终端并进入代码目录

2. **创建构建目录**：  
   创建独立目录以保持源码整洁：

   ```bash
   mkdir build
   cd build
   ```

3. **生成构建文件**：  
   运行CMake生成构建文件（如Makefile）：

   ```bash
   cmake -G "Ninja" ..
   ```
   或
   ```bash
   cmake -G "Unix Makefiles" ..
   ```

   此命令将读取父目录的`CMakeLists.txt`并生成所需构建文件。

4. **编译项目**：  
   使用生成的构建文件编译：
   
   ninja用户：
   ```bash
   ninja
   ```

   make用户：
   ```bash
   make
   ```
   （Windows下）
   ```bash
   mingw32-make
   ```

   编译后将生成可执行文件：`simplificator`、`differentiate`和`differentiate_simplified`（Windows下为`.exe`后缀）。

## 运行可执行文件

构建成功后，可在构建目录中运行生成的可执行文件。

1. **运行表达式简化器**：
   ```bash
   ./simplificator "<表达式>"
   ```

2. **运行求导程序**：
   ```bash
   ./differentiate "<表达式>" [<变量>]
   ```

3. **运行简化版求导程序**：
   ```bash
   ./differentiate_simplified "<表达式>" [<变量>]
   ```

## 清理构建

如需清理构建产物：

```bash
ninja clean
```
或
```bash
make clean
```
（Windows下）
```bash
mingw32-make clean
```

## 重新构建

修改源码后，在构建目录中重新运行`ninja`或`make`即可增量编译。

## CMake高级选项

- **指定构建类型**（Debug/Release）：

  ```bash
  cmake -DCMAKE_BUILD_TYPE=Debug ..
  ```
  或
  ```bash
  cmake -DCMAKE_BUILD_TYPE=Release ..
  ```

# <span id="release-chinese">二进制发布版</span>

提供**Windows-amd64**和**Linux-amd64**的预编译版本。

三个文件的功能已在第一部分提到，[跳转](#executable-info-chinese)

请注意，不应直接双击可执行文件，而需在终端中使用引号包裹参数运行。

可自行重命名下载文件。

## Windows

**编译器**：gcc 14.2.0 gcc version 14.2.0 (Rev2, Built by MSYS2 project)  
**下载链接**：
  - simplificator.exe：  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/uploads/2025/03/25/194735821579BopmsS.exe
  - differentiate.exe：  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/uploads/2025/03/25/1946518359xmXdGs08.exe
  - differentiate_simplified.exe：  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/uploads/2025/03/25/1947186110vu1XsdYF.exe

## Linux

**编译器**：gcc 12.2.0 (Debian 12.2.0-14)  
**下载链接**：
  - simplificator：  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/simplificator_linux
  - differentiate：  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/differentiate_linux
  - differentiate_simplified：  
    https://zjucube.oss-cn-hangzhou.aliyuncs.com/differentiate_simplified_linux