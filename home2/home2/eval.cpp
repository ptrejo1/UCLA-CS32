#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
#include "mazequeue.h"

using namespace std;

// check if char is operator
bool isOperator(char a){
    
    char ops[4] = {'+', '-', '*', '/'};
    for(int i = 0; i < 4; i++){
        if(a == ops[i]){
            return true;
        }
    }
    
    return false;
}

// check if char is parentheses or operator
bool isOperatorOrParen(char a){
    
    char ops[6] = {'(', ')', '+', '-', '*', '/'};
    for(int i = 0; i < 6; i++){
        if(a == ops[i]){
            return true;
        }
    }
    
    return false;
}

// check if char is parentheses
bool isParen(char a){
    
    char ops[2] = {'(', ')'};
    for(int i = 0; i < 2; i++){
        if(a == ops[i]){
            return true;
        }
    }
    
    return false;
}

// check operator precedence
bool lessOrEqualPrec(char a, char top){
    
    if ( (a == '*' || a == '/') && (top == '*' || top == '/') ) {
        return true;
    }
    
    if ( (a == '*' || a == '/') && (top == '+' || top == '-') ) {
        return false;
    }
    
    if ( (a == '+' || a == '-') && (top == '*' || top == '/') ) {
        return true;
    }
    
    if ( (a == '+' || a == '-') && (top == '+' || top == '-') ) {
        return true;
    }
    
    return false;
}

// Read char to apply corresponding operation
bool operation(int op1, int op2, char operation, int& res){
    
    switch (operation) {
        case '+': res = op1 + op2; break;
        case '-': res = op1 - op2; break;
        case '*': res = op1 * op2; break;
        case '/':
            if(op2 == 0) {
                return false;
            } else {
                res = op1 / op2;
                break;
            }
    }
    return true;
}

//   Evaluates an integer arithmetic expression
//   If infix is a syntactically valid infix integer expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values map), then postfix is set to the postfix
//   form of the expression; otherwise postfix may or may not be
//   changed, result is unchanged, and the function returns 1.  If
//   infix is syntactically valid but contains at least one lower
//   case letter operand that does not appear in the values map, then
//   result is unchanged and the function returns 2.  If infix is
//   syntactically valid and all its lower case operand letters
//   appear in the values map, then if evaluating the expression
//   (using for each letter in the expression the value in the map
//   that corresponds to it) attempts to divide by zero, then result
//   is unchanged and the function returns 3; otherwise, result is
//   set to the value of the expression and the function returns 0.
int evaluate(string infix, const Map& values, string& postfix, int& result){
    
    postfix = "";
    stack<char> ops;
    
    // strip whitespace
    infix.erase(std::remove(infix.begin(), infix.end(),' '), infix.end());
    
    // check for empty string
    if (infix.length() == 0) {
        return 1;
    }
    
    // Check if first and last characters are valid
    if (isOperator(infix[0]) || isOperator(infix[infix.length()-1])
        || infix[0] == ')' || infix[infix.length()-1] == '(') {
        return 1;
    }
    
    int parenCount = 0;
    for (int i = 0; i < infix.length(); i++) {
        char ch = infix[i];
        
        // check for uppercase letters
        if (isupper(ch)) {
            return 1;
        }
        
        // validation check for operands
        if (isalpha(ch) && (isalpha(infix[i+1]) || infix[i+1] == '(')) {
            return 1;
        }
        
        // validation check for operators
        if (isOperator(ch) && (infix[i+1] == ')' || isOperator(infix[i+1]))) {
            return 1;
        }
        
        // validation check for operators
        if (ch == '(' && (infix[i+1] == ')' || isOperator(infix[i+1]))) {
            return 1;
        }
        
        // validation check for operators
        if (ch == ')' && (infix[i+1] == '(' || isalpha(infix[i+1]))) {
            return 1;
        }
        
        // Infix to postfix conversion
        if(isalpha(ch)){
            postfix += ch;
        } else if (ch == '('){
            parenCount++;
            ops.push(ch);
        } else if (ch == ')'){
            parenCount++;
            while(ops.top() != '('){
                postfix += ops.top();
                ops.pop();
            }
            ops.pop();
        } else if(isOperator(ch)){
            while (!ops.empty() && ops.top() != '(' && lessOrEqualPrec(ch, ops.top()) ) {
                postfix += ops.top();
                ops.pop();
            }
            ops.push(ch);
        }
    }
    
    if(parenCount % 2 != 0) {
        return 1;
    }
    
    while (!ops.empty()) {
        postfix += ops.top();
        ops.pop();
    }
    
    // Evaluate postfix
    stack<int> vals;
    for (int i = 0; i < postfix.length(); i++) {
        char ch = postfix[i];
        int v;
        int operand1;
        int operand2;
        int temp;
        
        if(isalpha(ch)){
            if (!values.get(ch, v))
                return 2;
            else
                vals.push(v);
        } else {
            operand2 = vals.top();
            vals.pop();
            operand1 = vals.top();
            vals.pop();
            if (!operation(operand1, operand2, ch, temp))
                return 3;
            else
                vals.push(temp);
        }
    }
    
    result = vals.top();
    return 0;
}

int main() {
    
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  && pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
    
    // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  && pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  && pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  && pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  && pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  && pf == "a"  &&  answer == 3);
    cout << "Passed all tests" << endl;
    
     
    string maze[10] = {
        "XXXXXXXXXX",
        "X...X..X.X",
        "X..XX....X",
        "X.X.XXXX.X",
        "XXX......X",
        "X...X.XX.X",
        "X.X.X..X.X",
        "X.XXXX.X.X",
        "X..X...X.X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 4,3, 1,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
    
    
    return 0;
}