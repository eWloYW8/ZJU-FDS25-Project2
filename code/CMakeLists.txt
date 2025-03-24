# Set the minimum required version of CMake for this project
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(FDS25_Project2)

# Include directories for header files
include_directories(ast)            # Include headers from the 'ast' directory
include_directories(simplificator)  # Include headers from the 'simplificator' directory
include_directories(.)              # Include headers from the current directory

# Define an executable target named 'simplificator' and specify its source files
add_executable(simplificator 
    simplificator.c                # Main source file for the simplificator executable
    ast/ast.c                      # Source file for abstract syntax tree (AST) implementation
    ast/stack.c                    # Source file for stack implementation used in AST
    utils.c                        # Source file for utility functions
    ast/astmath.c                  # Source file for AST mathematical operations
    simplificator/one_layer.c      # Source file for one layer simplification
    simplificator/monomial.c       # Source file for monomial operations
)

# Define an executable target named 'differentiate' and specify its source files
add_executable(differentiate 
    differentiate.c                # Main source file for the differentiate executable
    ast/ast.c                      # Source file for abstract syntax tree (AST) implementation
    ast/stack.c                    # Source file for stack implementation used in AST
    utils.c                        # Source file for utility functions
    ast/astmath.c                  # Source file for AST mathematical operations
)

# Define an executable target named 'differentiate_simplified' and specify its source files
add_executable(differentiate_simplified 
    differentiate_simplified.c     # Main source file for the differentiate_simplified executable
    ast/ast.c                      # Source file for abstract syntax tree (AST) implementation
    ast/stack.c                    # Source file for stack implementation used in AST
    utils.c                        # Source file for utility functions
    ast/astmath.c                  # Source file for AST mathematical operations
    simplificator/one_layer.c      # Source file for one layer simplification
    simplificator/monomial.c       # Source file for monomial operations
)