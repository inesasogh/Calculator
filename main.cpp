#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <stack>
#include <queue>
enum TokenType {
	OPERANDS,
	OPERATORS,
	LEFT_PARTH,
	RIGHT_PARTH
};
struct Token
{
	std::string value;
	TokenType type;
};
std::vector<Token>  tokenization(const std::string& expression) {
	std::vector <Token> tokens;
	std::string Currtokens;
	for (char c : expression) {
		if (std::isdigit(c) || c == '.') {
			Currtokens += c;
		}
		else {
			if (!Currtokens.empty()) {
				tokens.push_back({ Currtokens,OPERANDS });
				Currtokens.clear();
			}
			switch (c)
			{
			case '+':
			case '-':
			case '*':
			case '/':
				tokens.push_back({ std::string(1,c),OPERATORS });
				break;
			case '(':
				tokens.push_back({ std::string(1,c),LEFT_PARTH });
				break;
			case ')':
				tokens.push_back({ std::string(1,c),RIGHT_PARTH });
				break;
			}
		}
	}
	if (!Currtokens.empty()) {
		tokens.push_back({ Currtokens,OPERANDS });
	}
	return tokens;
}
int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <expression>" << std::endl;
		return 1;
	}
	else {
		std::stack <std::string> RPNstack;
		std::queue <Token> RPNoutput;
		std::string expression = argv[1];
		std::vector<Token> tokens;
		tokens = tokenization(expression);
		for (const auto& token : tokens) {
			switch (token.type) {
			case OPERANDS:
				RPNoutput.push(token);
				break;
			case OPERATORS:
				if (token.value == "+") {
					if (RPNstack.empty()) {
						RPNstack.push(token.value);
						continue;
					}
					else if ((RPNstack.top() == "*" || RPNstack.top() == "/" || RPNstack.top() == "-" || RPNstack.top() == "+")) {
						RPNoutput.push({ RPNstack.top(),OPERATORS });
						RPNstack.pop();
					}
					RPNstack.push(token.value);
				}
				else if (token.value == "-") {
					if (RPNstack.empty()) {
						RPNstack.push(token.value);
						continue;
					}
					else if (RPNstack.top() == "*" || RPNstack.top() == "/" || RPNstack.top() == "-" || RPNstack.top() == "+") {
						RPNoutput.push({ RPNstack.top(), OPERATORS });
						RPNstack.pop();
					}
					RPNstack.push(token.value);
				}
				else if (token.value == "*") {
					if (RPNstack.empty()) {
						RPNstack.push(token.value);
						continue;
					}
					else if (RPNstack.top() == "*" || RPNstack.top() == "/") {
						RPNoutput.push({ RPNstack.top(), OPERATORS });
						RPNstack.pop();
					}
					RPNstack.push(token.value);
				}
				else if (token.value == "/") {
					if (RPNstack.empty()) {
						RPNstack.push(token.value);
						continue;
					}
					else if (RPNstack.top() == "*" || RPNstack.top() == "/") {
						RPNoutput.push({ RPNstack.top(),OPERATORS });
						RPNstack.pop();
					}
					RPNstack.push(token.value);
				}
				break;
			case LEFT_PARTH:
				RPNstack.push(token.value);
				break;
			case RIGHT_PARTH:
				while (RPNstack.top() != "(") {
					RPNoutput.push({ RPNstack.top(),OPERATORS });
					RPNstack.pop();
				}
				RPNstack.pop();
				break;
			}
		}
		while (!RPNstack.empty()) {
			RPNoutput.push({ RPNstack.top(),OPERATORS });
			RPNstack.pop();
		}
		std::stack<std::string> result;
		while (!RPNoutput.empty()) {
			switch (RPNoutput.front().type)
			{
			case OPERANDS:
				result.push(RPNoutput.front().value);
				break;
			case OPERATORS:
				if (RPNoutput.front().value == "+") {
					int first, second;
					first = stoi(result.top());
					result.pop();
					second = stoi(result.top());
					result.pop();
					result.push(std::to_string(first + second));
				}
				else if (RPNoutput.front().value == "-") {
					int first, second;
					first = stoi(result.top());
					result.pop();
					second = stoi(result.top());
					result.pop();
					result.push(std::to_string(second - first));
				}
				else if (RPNoutput.front().value == "*") {
					int first, second;
					first = stoi(result.top());
					result.pop();
					second = stoi(result.top());
					result.pop();
					result.push(std::to_string(first * second));
				}
				else if (RPNoutput.front().value == "/") {
					int first, second;
					first = stoi(result.top());
					result.pop();
					second = stoi(result.top());
					result.pop();
					result.push(std::to_string(second / first));
				}
				break;
			}
			RPNoutput.pop();
		}
		std::cout << result.top();
	}
	return 0;
}