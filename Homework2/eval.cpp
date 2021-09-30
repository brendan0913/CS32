#include <string>
#include <stack>
#include <iostream>

using namespace std;

void removeSpaces(string& infix);
bool corners(string infix);
bool numOfParentheses(string infix);
bool consOps(string infix);
bool valid(string infix);
int precedenceCheck(char c);

int evaluate(string infix, string& postfix, bool& result)
// Evaluates a boolean expression
//   If infix is a syntactically valid infix boolean expression,
//   then set postfix to the postfix form of that expression, set
//   result to the value of the expression, and return zero.  If
//   infix is not a syntactically valid expression, return 1; in
//   that case, postfix may or may not be changed, but result must
//   be unchanged
{
	removeSpaces(infix);
	if (!numOfParentheses(infix) || !corners(infix) || !consOps(infix) || !valid(infix))
	{
		return 1;
	}
	else
	{
		postfix = "";
		stack<char> ops;
		for (size_t i = 0; i < infix.size(); i++)
		{
			char c = infix.at(i);
			switch (c)
			{
			case 'T':
			case 'F':
				postfix += c;
				break;
			case '(':
			case '!':
				ops.push(c);
				break;
			case ')':
				// pop stack until matching '('
				while (ops.top() != '(')
				{
					postfix += ops.top();
					ops.pop();
				}
				ops.pop();  // remove the '('
				break;
			case '&':
			case '^':
				while (!ops.empty() && ops.top() != '(' && (precedenceCheck(c) <= precedenceCheck(ops.top())))
				{
					postfix += ops.top();
					ops.pop();
				}
				ops.push(c);
				break;
			default:
				break;
			}
		}
		while (!ops.empty())
		{
			postfix += ops.top();
			ops.pop();
		}
	}
	stack<char> operands;
	for (size_t j = 0; j < postfix.size(); j++)
	{
		char ch = postfix.at(j);
		if (ch == 'T' || ch == 'F')
		{
			operands.push(ch);
		}
		else if (ch == '!') // ch is a unary operator
		{
			char op = operands.top();
			operands.pop();
			if (op == 'T')
			{
				operands.push('F');
			}
			if (op == 'F')
			{
				operands.push('T');
			}
		}
		else // ch is a binary operator
		{
			char operand2 = operands.top();
			operands.pop();
			char operand1 = operands.top();
			operands.pop();
			if (ch == '&')
			{
				if (operand1 == 'F' || operand2 == 'F')
				{
					operands.push('F');
				}
				else
				{
					operands.push('T');
				}
			}
			if (ch == '^')
			{
				if (operand1 != operand2)
				{
					operands.push('T');
				}
				else
				{
					operands.push('F');
				}
			}
		}
	}
	if (operands.top() == 'T')
	{
		result = true;
	}
	else
	{
		result = false;
	}
	return 0;
}

void removeSpaces(string& infix)
{
	string s = "";
	for (size_t j = 0; j < infix.size(); j++)
	{
		if (infix.at(j) != ' ')
		{
			s += infix.at(j);
		}
	}
	infix = s;
}

bool numOfParentheses(string infix)
{
	if (infix == "") { return false; }
	int open = 0;
	int closed = 0;
	for (size_t i = 0; i < infix.size(); i++)
	{
		if (infix.at(i) == '(')
		{
			open++;
		}
		if (infix.at(i) == ')')
		{
			closed++;
		}
		if (closed > open)
		{
			return false;
		}
	}
	int open2 = 0;
	int closed2 = 0;
	for (int i = infix.size()-1; i >= 0; i--)
	{
		if (infix.at(i) == '(')
		{
			open2++;
		}
		if (infix.at(i) == ')')
		{
			closed2++;
		}
		if (open2>closed2)
		{
			return false;
		}
	}
	return open == closed;
}

bool corners(string infix)
{
	if (infix.size() == 0)
	{
		return false;
	}
	if (infix.size() == 1 && (infix.at(0) == '!' || infix.at(0) == '^' || infix.at(0) == '&' || infix.at(0) == '(' || infix.at(0) == ')'))
	{
		return false;
	}
	if (infix.at(0) == '&' || infix.at(0) == '^' || infix.at(0) == ')')
	{
		return false;
	}
	if (infix.at(infix.size() - 1) == '&' || infix.at(infix.size() - 1) == '^' || infix.at(infix.size() - 1) == '!')
	{
		return false;
	}
	if (infix.at(0) == ')' || infix.at(infix.size() - 1) == '(')
	{
		return false;
	}
	return true;
}


bool consOps(string infix)
{
	for (size_t i = 0; i < infix.size()-1; i++)
	{
		if (infix.at(i) == '(' && (infix.at(i + 1) == ')' || infix.at(i+1) == '^' || infix.at(i+1) == '&'))
		{
			return false;
		}
		if (infix.at(i) == ')' && infix.at(i + 1) == '(')
		{
			return false;
		}
		if (infix.at(i) == ')' && (infix.at(i + 1) == 'T' || infix.at(i + 1) == 'F' || infix.at(i+1) == '!'))
		{
			return false;
		}
		if ((infix.at(i) == '!' || infix.at(i+1)== '(') && (infix.at(i + 1) == '&' || infix.at(i + 1) == '^'))
		{
			return false;
		}
		if ((infix.at(i) == '&' || infix.at(i) == '^') && (infix.at(i + 1) == '&' || infix.at(i + 1) == '^' || infix.at(i+1) == ')'))
		{
			return false;
		}
		if ((infix.at(i) == 'T' || infix.at(i) == 'F') && (infix.at(i + 1) == '!' || infix.at(i + 1) == '('))
		{
			return false;
		}
		if ((infix.at(i) == 'T' || infix.at(i) == 'F') && (infix.at(i + 1) == 'T' || infix.at(i + 1) == 'F'))
		{
			return false;
		}
	}
	for (size_t i = 1; i < infix.size(); i++)
	{
		if ((infix.at(i) == 'T' || infix.at(i) == 'F') && (infix.at(i - 1) != '^' && infix.at(i - 1) != '!' && infix.at(i - 1) != '&' && infix.at(i - 1) != '('))
		{
			return false;
		}
		if (infix.at(i) == '!' && (infix.at(i-1) != '^' && infix.at(i-1) != '!' && infix.at(i-1) != '&' && infix.at(i-1) != '('))
		{
			return false;
		}
		if ((infix.at(i) == '^' || infix.at(i) == '&') && (infix.at(i - 1) != 'T' && infix.at(i - 1) != 'F' && infix.at(i - 1) != ')'))
		{
			return false;
		}
		if (infix.at(i) == '(' && (infix.at(i - 1) != '!' && infix.at(i - 1) != '&' && infix.at(i - 1) != '^' && infix.at(i - 1) != '('))
		{
			return false;
		}
		if (infix.at(i) == ')' && (infix.at(i - 1) != 'T' && infix.at(i - 1) != 'F' && infix.at(i - 1) != ')'))
		{
			return false;
		}
	}
	return true;
}

bool valid(string infix)
{
	for (size_t i = 0; i < infix.size(); i++)
	{
		if (!(infix.at(i) == 'T' || infix.at(i) == 'F' || infix.at(i) == '!' || infix.at(i) == '&' || infix.at(i) == '^' || infix.at(i) == '(' || infix.at(i) == ')'))
		{
			return false;
		}
	}
	return true;
}

int precedenceCheck(char c)
{
	int i = -1;
	switch (c)
	{
	case '!':
		i = 3;
		break;
	case '&':
		i = 2;
		break;
	case '^':
		i = 1;
		break;
	default:
		break;
	}
	return i;
}