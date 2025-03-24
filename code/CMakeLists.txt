cmake_minimum_required(VERSION 3.10)

project(FDS25_Project2)

include_directories(ast)

include_directories(simplificator)

include_directories(.)

add_executable(simplificator simplificator.c ast/ast.c ast/stack.c utils.c ast/astmath.c simplificator/one_layer.c simplificator/monomial.c)

add_executable(differentiate differentiate.c ast/ast.c ast/stack.c utils.c ast/astmath.c simplificator/one_layer.c simplificator/monomial.c)

add_executable(differentiate_simplified differentiate_simplified.c ast/ast.c ast/stack.c utils.c ast/astmath.c simplificator/one_layer.c simplificator/monomial.c)