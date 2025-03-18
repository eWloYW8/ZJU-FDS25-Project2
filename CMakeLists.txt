cmake_minimum_required(VERSION 3.10)

project(FDS25_Project2)

include_directories(code)

add_executable(test code/test.c code/ast.c code/ads/stack.c code/utils.c code/astmath.c code/simplificator/one_layer.c code/simplificator/monomial.c)

add_executable(test2 code/test2.c code/ast.c code/ads/stack.c code/utils.c code/astmath.c code/simplificator/one_layer.c code/simplificator/monomial.c)
