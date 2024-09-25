#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <iostream>         // Include the standard stream library
#include <stack>            // Include stack data structure
#include <string>           // Include string library
#include <cmath>            // Include mathematical functions
#include <unordered_set>    // Include for handling unique variables
#include <cctype>           // Include to handle digit characters
#include <vector>           // Include the vector library


using namespace std;        // Avoid writing "std::" repeatedly

// Class definition for math operations
class MathSymbols {
private:
    // Function to check precedence of operators
    int precedence(const string& symbol) {
        if (symbol == "sin" || symbol == "cos" || symbol == "log") {
            return 5; // Highest precedence
        } else if (symbol == "^") {
            return 4;       // Exponent has next highest precedence
        } else if (symbol == "*" || symbol == "/") {
            return 3;       // Multiplication and division
        } else if (symbol == "+" || symbol == "-") {
            return 2;       // Addition and subtraction
        } else if (symbol == "(") {
            return 1;       // Parentheses
        } else {
            return 0;       // Invalid character
        }
    }

    // Function to check if a string is a valid number
    bool isNumber(const string& str) {
        char* end = nullptr;
        strtod(str.c_str(), &end);          // Convert string to double
        return end == &str[str.size()];     // Check if the entire string is a number
    }

    // Function to check if a character is an operand (digit or decimal point)
    bool isOperand(char ch) {
        return isdigit(ch) || ch == '.';    // Return true if it's a digit or decimal point
    }

    // Function to check if the expression is valid with correct operators and parentheses
    bool isValidExpression(const string& infix) {
        int bracket_counts = 0;     // Counter for parentheses
        char last_char = '\0';      // Track last character

        for (size_t i = 0; i < infix.length(); i++) {
            char current = infix[i];
            if (current == '(') bracket_counts++;           // Increment for opening parenthesis
            else if (current == ')') bracket_counts--;      // Decrement for closing parenthesis
            if (bracket_counts < 0) return false;           // If closing comes before opening, return false

            // Check for invalid characters
            if (!isdigit(current) && !isalpha(current) && !isspace(current) &&
                current != '+' && current != '-' && current != '*' &&
                current != '/' && current != '^' && current != '(' && current != ')' &&
                current != '.') {
                return false;  // Return false for invalid characters
            }

            // Check for consecutive operators (e.g., ++, --)
            if ((current == '+' || current == '-' || current == '*' || current == '/' || current == '^') &&
                (last_char == '+' || last_char == '-' || last_char == '*' || last_char == '/' || last_char == '^')) {
                return false;  // Return false for consecutive operators
            }

            if (!isspace(current)) last_char = current;
        }
        return bracket_counts == 0;
    }

    // Split expression into parts (numbers, operators, functions)
    vector<string> splitIntoParts(const string& str) {
        vector<string> split;
        size_t i = 0;
        while (i < str.length()) {
            if (isspace(str[i])) {
                i++;
                continue;
            }
            if (isOperand(str[i]) || (str[i] == '-' && i + 1 < str.length() && isdigit(str[i + 1]))) {
                string num;
                while (i < str.length() && (isOperand(str[i]))) {
                    num += str[i];
                    i++;
                }
                split.push_back(num);
            } else if (isalpha(str[i])) {
                string name;
                while (i < str.length() && isalpha(str[i])) {
                    name += str[i];
                    i++;
                }
                if (name == "pi") {
                    split.push_back(to_string(M_PI));
                } else {
                    split.push_back(name);
                }
            } else {
                split.push_back(string(1, str[i]));
                i++;
            }
        }
        return split;
    }

    // Helper function to apply an operator to two operands
    double applyOperator(double a, double b, const string& op) {
        if (op == "+") return a + b;
        else if (op == "-") return a - b;
        else if (op == "*") return a * b;
        else if (op == "/") return a / b;
        else if (op == "^") return pow(a, b);
        else return 0;
    }

    // Function to apply the operator or function on top of the ops stack
    bool applyTopOperator(stack<double>& values, stack<string>& ops) {
        string op = ops.top();
        ops.pop();

        if (op == "sin" || op == "cos" || op == "log") {
            if (values.empty()) {
                cout << "Error: Insufficient operands for function " << op << endl;
                return false;
            }
            double val = values.top();
            values.pop();
            if (op == "sin") values.push(sin(val)); // Argument is in radians
            else if (op == "cos") values.push(cos(val));
            else if (op == "log") values.push(log10(val));
        } else {
            if (values.size() < 2) {
                cout << "Error: Insufficient operands for operator " << op << endl;
                return false;
            }
            double val2 = values.top(); values.pop();
            double val1 = values.top(); values.pop();
            values.push(applyOperator(val1, val2, op));
        }
        return true;
    }

    // Evaluate infix expression
    double evaluateInfix(const string& infix) {
        stack<double> values;   // Stack for numbers
        stack<string> ops;      // Stack for operators and functions
        vector<string> split = splitIntoParts(infix);

        for (size_t i = 0; i < split.size(); i++) {
            string token = split[i];

            // If token is a number, push it to the values stack
            if (isNumber(token)) {
                values.push(stod(token));
            }
                // If token is a function like sin, cos, log, push it to the ops stack
            else if (token == "sin" || token == "cos" || token == "log") {
                ops.push(token);
            }
                // If token is an opening parenthesis, push it to ops stack
            else if (token == "(") {
                ops.push("(");
            }
                // If token is a closing parenthesis, solve the entire parenthesis
            else if (token == ")") {
                while (!ops.empty() && ops.top() != "(") {
                    if (!applyTopOperator(values, ops)) return 0;
                }
                ops.pop(); // Remove the '('
                // Apply function if there is one on top of the ops stack
                if (!ops.empty() && (ops.top() == "sin" || ops.top() == "cos" || ops.top() == "log")) {
                    if (!applyTopOperator(values, ops)) return 0;
                }
            }
                // If token is an operator, process it
            else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
                while (!ops.empty() && ((precedence(ops.top()) > precedence(token)) ||
                                        (precedence(ops.top()) == precedence(token) && token != "^"))) {
                    if (!applyTopOperator(values, ops)) return 0;
                }
                ops.push(token);  // Push the current operator onto the ops stack
            }
            else {
                cout << "Invalid token: " << token << endl;
                return 0;
            }
        }

        // Apply remaining operators
        while (!ops.empty()) {
            if (!applyTopOperator(values, ops)) return 0;
        }

        return values.top(); // Return the final result
    }

public:
    // Convert infix to postfix
    string infixToPostfix(const string& infix, unordered_set<string>& variables) {
        stack<string> stack_list;
        string postfix;
        vector<string> split = splitIntoParts(infix);

        for (size_t i = 0; i < split.size(); i++) {
            string token = split[i];

            if (isNumber(token) || token == to_string(M_PI)) {
                postfix += token + ' ';
            }
            else if (isalpha(token[0])) {
                if (token == "sin" || token == "cos" || token == "log") {
                    if (i + 1 < split.size() && split[i + 1] == "(") {
                        stack_list.push(token);
                        stack_list.push(split[++i]);
                    } else {
                        cout << "Error: Expected '(' after function '" << token << "'." << endl;
                        return "";
                    }
                } else {
                    postfix += token + ' ';
                    variables.insert(token);
                }
            }
            else if (token == "(") {
                stack_list.push(token);
            }
            else if (token == ")") {
                while (!stack_list.empty() && stack_list.top() != "(") {
                    postfix += stack_list.top() + ' ';
                    stack_list.pop();
                }
                if (stack_list.empty()) {
                    cout << "Error: Mismatched parentheses." << endl;
                    return "";
                }
                stack_list.pop();
                if (!stack_list.empty() && (stack_list.top() == "sin" || stack_list.top() == "cos" || stack_list.top() == "log")) {
                    postfix += stack_list.top() + ' ';
                    stack_list.pop();
                }
            }
            else {
                while (!stack_list.empty() && precedence(stack_list.top()) >= precedence(token)) {
                    postfix += stack_list.top() + ' ';
                    stack_list.pop();
                }
                stack_list.push(token);
            }
        }

        while (!stack_list.empty()) {
            if (stack_list.top() == "(") {
                cout << "Error: Mismatched parentheses." << endl;
                return "";
            }
            postfix += stack_list.top() + ' ';
            stack_list.pop();
        }
        return postfix;
    }

    // Evaluate postfix expression
    double calculatePostfix(const string& postfix) {
        stack<double> stack_2;
        vector<string> split = splitIntoParts(postfix);

        for (const string& index : split) {
            if (isNumber(index)) {
                stack_2.push(stod(index));
            }
            else if (index == to_string(M_PI)) {
                stack_2.push(M_PI);
            }
            else if (index == "sin" || index == "cos" || index == "log") {
                if (stack_2.empty()) {
                    cout << "Error: Insufficient operands for function " << index << endl;
                    return 0;
                }
                double operand = stack_2.top(); stack_2.pop();
                if (index == "sin") stack_2.push(sin(operand)); // Argument is in radians
                else if (index == "cos") stack_2.push(cos(operand));
                else if (index == "log") stack_2.push(log10(operand));
            }
            else if (index == "+" || index == "-" || index == "*" || index == "/" || index == "^") {
                if (stack_2.size() < 2) {
                    cout << "Error: Insufficient operands for operator " << index << endl;
                    return 0;
                }
                double op2 = stack_2.top(); stack_2.pop();
                double op1 = stack_2.top(); stack_2.pop();
                if (index == "+") stack_2.push(op1 + op2);
                else if (index == "-") stack_2.push(op1 - op2);
                else if (index == "*") stack_2.push(op1 * op2);
                else if (index == "/") stack_2.push(op1 / op2);
                else if (index == "^") stack_2.push(pow(op1, op2));
            }
            else {
                cout << "Error: Undefined variable or function '" << index << "' in evaluation." << endl;
                return 0;
            }
        }
        return stack_2.empty() ? 0 : stack_2.top();
    }

    // Main function to calculate infix and postfix results
    void calculate() {
        string infix_expo, postfix_expo;

        cout << "\n+--------------------------------------------------------------+" << endl;
        cout << "|         Welcome to Infix to Postfix converter!                |" << endl;
        cout << "+--------------------------------------------------------------+" << endl;
        cout << "| Please enter a mathematical expression in infix notation.     |" << endl;
        cout << "|                                                               |" << endl;
        cout << "| Supported operations: +, -, *, /, ^                           |" << endl;
        cout << "| Supported functions: sin(x), cos(x), log(x), pi               |" << endl;
        cout << "|                                                               |" << endl;
        cout << "| Important guidelines:                                         |" << endl;
        cout << "| 1. Use parentheses for function arguments, e.g., sin(90).     |" << endl;
        cout << "| 2. To represent sin^2(90), write (sin(90))^2.                 |" << endl;
        cout << "| 3. Multiplication must be explicit, e.g., 2 * x instead of 2x.|" << endl;
        cout << "| 4. Ensure all parentheses are properly closed.                |" << endl;
        cout << "|                                                               |" << endl;
        cout << "| Example of a valid expression:                                |" << endl;
        cout << "| (3 * log(100) + (sin(90))^2 - cos(log(1000)) + 2 - 3 * 4 + 12)|" << endl;
        cout << "+--------------------------------------------------------------+" << endl;
        cout << "Please enter your expression below:" << endl;

        getline(cin, infix_expo);

        if (!isValidExpression(infix_expo)) {
            cout << "Invalid expression! Please use valid operators and ensure parentheses are matched." << endl;
            return;
        }

        unordered_set<string> variables;
        postfix_expo = infixToPostfix(infix_expo, variables);

        if (postfix_expo.empty()) {
            return;
        }

        cout << "\nThe postfix expression is: " << postfix_expo << endl;

        if (variables.empty()) {
            double infix_result = evaluateInfix(infix_expo);
            double postfix_result = calculatePostfix(postfix_expo);

            cout << "The result of the infix expression is: " << infix_result << endl;
            cout << "The result of the postfix expression is: " << postfix_result << endl;
        } else {
            cout << "Note: Expression contains variables. Evaluation is not performed." << endl;
        }
    }


};

int main() {
    MathSymbols exprProcessor;  // Create an object of the MathSymbols class
    exprProcessor.calculate();  // Call the calculate function to start the operation
    return 0;
}
