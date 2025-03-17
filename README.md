# Introduction

这是 浙江大学 2024-2025 学年 春、夏 学期 CS1018F 数据结构基础(FDS) 课程的 Project 2 个人仓库

本人姓名+学号+身份证号字符串（格式如"张三+3212345678+130921199202218173"）的 sha256 值为 2CB76FBEA3FC658546864B3C6E4D80FB6C8B776FB520C0630C422A8BA3241C5D

此仓库将保持 private 状态，确保不违反诚信守则

# Usage

You can build this project with CMake.

To be finished...

# Problem

## Hard Autograd for Algebraic Expressions
The application of automatic differentiation technology in frameworks such as torch and tensorflow has greatly facilitated people's implementation and training of deep learning algorithms based on backpropagation. Now, we hope to implement an automatic differentiation program for algebraic expressions.

### Input Format
First, input an infix expression composed of the following symbols.
**Operators**

**Type** | **Examples** | **Notes**
---- | -------- | -----
Bracket | ( ) |
Power | ^     |
Multiplication & Division | * / |
Addition & Subtraction | + - |
Argument separator | , | **optional**, only used as argument separators in multivariate functions

The above operators are arranged in order of operator precedence from top to bottom. For example, ```a+b^c*d``` will be considered the same as ```a + ( (b ^ c) * d )```.

**Mathematical Functions** (bonus)

**Function** | **Description**
-------- | -----
ln(A) log(A, B) | logarithm.  ln(A) represents the natural logarithm of A, and log(A, B) represents the logarithm of B based on A.
cos(A) sin(A) tan(A) | basic trigonometric functions.
pow(A, B) exp(A) | exponential functions. pow(A, B) represents the B power of A, and exp(A) represents the natural exponential of A.

**Operands**

**Type** | **Examples** | **Notes**
---- | -------- | -----
Literal constant | 2 3 0 -5 | Just consider integers consisting of pure numbers and minus signs.
Variable | ex cosval xy xx | Considering the above \"mathematical functions\" as reserved words, identifiers (strings of lowercase English letters) that are not reserved words are called variables.

### Output Method

For each **variable** (as defined above) that appears in the expression, describe an arithmetic expression that represents the derivative of the input algebraic expression with respect to that variable, using the operators, mathematical functions, and operands defined in the input form.
The output is arranged according to the **lexicographical order** of the variables. For each line print two strings, which are each variable and the corresponding derivative function. Separate the two strings with ```:```.

### Requirements

- Process a given algebraic expression containing **operators** and **operands**, and output the algebraic expression of the derivative function of the original expression with respect to each variable.
- It is only necessary to ensure that the output expression is **identical** to the correct derivative function, and no simplification or expansion of the expression is required.
- Implemented in C/C++, only the standard library can be used.
- The code is highly readable or well-commented. Please make sure the source files can be opened correctly with utf-8 encoding.
- **Bonus**
—Not using complex containers provided in STL
—Support for expressions containing mathematical functions
—Simplify an algebraic expression to reduce the length of the result by applying at least two rules.

### Hints
- Build expression trees from infix expressions.
- Simple input cases:
 - ```a+b^c*d```
 outputs:
```
 a: 1
 b: c*b^(c-1)*d
 c: ln(b)*b^c*d
 d: b^c
```
 - ```a*10*b+2^a/a```
 outputs:
```
 a: 10*b-2^a/a^2+2^a*ln(2)/a
 b: a*10
```
 - ```xx^2/xy*xy+a^a``` (Note that ```xx``` and ```xy``` are treated as a single variable respectively)
 outputs:
```
 a: a^a*(1+ln(a))
 xx: 2*xx
 xy: 0
```
- More input cases
  - ```x*ln(y)```
  - ```x*ln(x*y)+y*cos(x)+y*sin(2*x)```
  - ```log(a,b)/log(c,a)```

### Grading Policy:
* Chapter 1: Introduction (6 pts.)
* Chapter 2: Algorithm Specification (12 pts.)
* Chapter 3: Testing Results (20 pts.)
* Chapter 4: Analysis and Comments (10 pts.)
* Write the program (50 pts.) with sufficient comments.
* Overall style of documentation (2 pts.)

**Note:** Anyone who does excellent job on solving the Bonus problem will gain an extra 5% of his/her points."
