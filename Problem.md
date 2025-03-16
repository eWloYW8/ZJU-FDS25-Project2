# Automatic Differentiation for Algebraic Expressions

The application of automatic differentiation technology in frameworks such as Torch and TensorFlow has greatly facilitated the implementation and training of deep learning algorithms based on backpropagation. Now, we aim to implement an automatic differentiation program for algebraic expressions.

## Input Format

First, input an infix expression composed of the following symbols.

### Operators

| Type                     | Examples | Notes |
|--------------------------|---------|-------|
| Bracket                 | `( )`   |       |
| Power                   | `^`     |       |
| Multiplication & Division | `* /`   |       |
| Addition & Subtraction   | `+ -`   |       |
| Argument separator      | `,`     | Optional, only used as argument separators in multivariate functions |

The above operators are arranged in order of operator precedence from top to bottom. For example, `a+b^c*d` will be considered the same as `a + ((b ^ c) * d)`.

### Mathematical Functions (Bonus)

| Function | Description |
|----------|-------------|
| `ln(A)`, `log(A, B)` | Logarithm functions. `ln(A)` represents the natural logarithm of `A`, and `log(A, B)` represents the logarithm of `B` based on `A`. |
| `cos(A)`, `sin(A)`, `tan(A)` | Basic trigonometric functions. |
| `pow(A, B)`, `exp(A)` | Exponential functions. `pow(A, B)` represents `A` raised to the power of `B`, and `exp(A)` represents the natural exponential of `A`. |

### Operands

| Type               | Examples         | Notes |
|--------------------|-----------------|-------|
| Literal constant  | `2 3 0 -5`       | Only consider integers consisting of pure numbers and minus signs. |
| Variable          | `ex cosval xy xx` | Reserved words include the above "mathematical functions." Identifiers (strings of lowercase English letters) that are not reserved words are treated as variables. |

## Output Method

For each variable (as defined above) appearing in the expression, output an arithmetic expression that represents the derivative of the input algebraic expression with respect to that variable. The output should follow these rules:

- Output variables in **lexicographical order**.
- For each variable, print two strings:
  - The variable name.
  - The corresponding derivative function.
- Separate the two strings with `:`.

### Example Output Format

```

a: 1
b: c\*b^(c-1)\*d
c: ln(b)*b^c*d
d: b^c

```

## Requirements

- Process a given algebraic expression containing **operators** and **operands**.
- Output the **derivative expression** of the original expression with respect to each variable.
- Ensure correctness of the derivative output without requiring simplification or expansion.
- Implement the solution in **C/C++** using only the **standard library**.
- The code should be **highly readable** or **well-commented**.
- Ensure source files can be opened correctly with **UTF-8 encoding**.

### Bonus

- **Avoid using complex containers** from STL.
- **Support expressions with mathematical functions**.
- **Simplify algebraic expressions** by applying at least two rules to reduce the result length.

## Hints

1. **Build an expression tree** from infix expressions.
2. **Simple input cases:**

   **Input:**  
```

a+b^c\*d

```
**Output:**  
```

a: 1
b: c\*b^(c-1)\*d
c: ln(b)*b^c*d
d: b^c

```

**Input:**  
```

a*10*b+2^a/a

```
**Output:**  
```

a: 10*b-2^a/a^2+2^a*ln(2)/a
b: a\*10

```

**Input:**  
```

xx^2/xy\*xy+a^a

```
**Output:**  
```

a: a^a\*(1+ln(a))
xx: 2\*xx
xy: 0

```

3. **More input cases:**
- `x*ln(y)`
- `x*ln(x*y)+y*cos(x)+y*sin(2*x)`
- `log(a,b)/log(c,a)`

## Grading Policy

| Chapter | Description | Points |
|---------|------------|--------|
| **Chapter 1** | Introduction | 6 pts |
| **Chapter 2** | Algorithm Specification | 12 pts |
| **Chapter 3** | Testing Results | 20 pts |
| **Chapter 4** | Analysis and Comments | 10 pts |
| **Code Implementation** | With sufficient comments | 50 pts |
| **Documentation Style** | Overall presentation | 2 pts |

**Bonus:**  
- Outstanding solutions solving the bonus problem will gain **an extra 5%**.
