cmake_minimum_required(VERSION 3.10)

project(FDS25_Project2)

include_directories(code)

add_executable(test code/test.c code/ast.c code/stack.c code/utils.c code/astmath.c code/simplificator/one_layer.c)
