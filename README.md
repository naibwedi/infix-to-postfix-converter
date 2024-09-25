### Mathematical Expression Evaluator
This program evaluates complex mathematical expressions provided by the user. It supports standard arithmetic operators, trigonometric functions (sin, cos), logarithmic functions (log), and the constant pi. The program first converts the expression from infix notation (standard math notation) to postfix notation (which is easier to evaluate) and then evaluates the result.

## Design (GUI)
![image](https://github.com/user-attachments/assets/f3e5dcf0-c919-46e0-be9b-1cdc082ac07f)

## Features
Infix-to-Postfix Conversion: Converts input expressions to postfix notation, making it easier to handle operator precedence.
Supported Arithmetic Operations: Addition (+), Subtraction (-), Multiplication (*), Division (/), and Exponentiation (^).
Supported Functions: Trigonometric (sin, cos), Logarithmic (log), and the constant pi.
Variable Detection: Capable of detecting variables (e.g., x, y), but does not evaluate them.

## How It Works
Infix to Postfix Conversion
In an infix expression, operators appear between operands, e.g., 3 * log(100) + sin^2(90). The program converts this to postfix notation, where operators appear after their operands, e.g., 3 100 log * 90 sin ^ 2 +. This is done using a stack to manage operator precedence and parentheses.

## Postfix Evaluation
Once the expression is converted to postfix, the program evaluates it by processing the postfix expression from left to right. Numbers are pushed onto a stack, and when an operator is encountered, the necessary operands are popped off the stack, the operation is performed, and the result is pushed back onto the stack.

## Supported Mathematical Functions
sin(x): Returns the sine of x (in radians).
cos(x): Returns the cosine of x (in radians).
log(x): Returns the base-10 logarithm of x.
pi: The constant π (approximately 3.14159).

## Program Structure
The program is organized into several key components:

#Libraries Used:
<iostream>: Handles input and output.
<stack>: Used for storing operators and intermediate results.
<string>: For handling the input expression as a string.
<cmath>: Provides mathematical functions (sin, cos, log) and constants like M_PI.
<unordered_set>: Stores variables in the expression.
<cctype>: Used for character validation (isdigit).
<vector>: Stores parts of the expression dynamically during parsing.
Class: MathSymbols
This class handles all the logic for:

Precedence Handling: Determines operator precedence (e.g., ^ is evaluated before * or /).
Infix-to-Postfix Conversion: Converts the input expression to postfix using a stack to manage precedence.
Postfix Evaluation: Evaluates the value of the postfix expression.
Key Functions:
precedence(const string& symbol): Determines operator precedence.
check_numbers(const string& str): Checks if a string represents a number.
split_into_parts(const string& str): Splits the input expression into manageable parts (numbers, operators, functions).
infixToPostfix(const string& infix, unordered_set<string>& variables): Converts the infix expression to postfix.
calculate_postfix(const string& postfix): Evaluates the postfix expression using a stack.
calculate(): Main function that takes user input, validates the expression, converts it to postfix, evaluates, and displays the result.
Main Function
cpp
Copy code
int main() {
    MathSymbols exprProcessor;
    exprProcessor.calculate();
    return 0;
}
The main() function creates an instance of the MathSymbols class and calls calculate() to run the program.

Usage
Input: You will be prompted to input a mathematical expression in infix notation. Example:

scss
Copy code
3 * log(100) + sin^2(90) - cos(log(1000)) + 2 - 3 * 4 + 12
Output: The program will display the postfix conversion of the expression and the final result. Example:

r
Copy code
The postfix expression is: 3 100 log * 90 sin ^ 2 + 1000 log cos - 2 + 3 4 * - 12 +
The result is: <result>
Final Notes
This program demonstrates how to use infix-to-postfix conversion and stack-based evaluation methods to handle mathematical expressions efficiently.

