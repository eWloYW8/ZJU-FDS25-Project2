cmake_minimum_required(VERSION 3.10)

project(FDS25_Project2)

include_directories(code)

add_executable(test code/test/test.c code/ast.c code/ads/stack.c code/utils.c code/astmath.c code/simplificator/one_layer.c)

add_executable(test2 code/test/test2.c code/ast.c code/ads/stack.c code/utils.c code/astmath.c code/simplificator/one_layer.c code/simplificator/expressionobj.c code/simplificator/polynomial.c code/simplificator/monomial.c)