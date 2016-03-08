//main.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include "main.h"

using namespace std;
ifstream in;

//vector of identifier keywords 
const string idArr[] = {"let", "in", "fn", "where", "aug", "not", "or", "gr", "ge", "ls", "le", "eq", "ne", "true", "false", "nil", "dummy", "within", "and", "rec", "list"};
vector<string> identifiers (idArr, idArr + sizeof(idArr) / sizeof(idArr[0]));

//vector of punctuations
const string punctArr[] = {"(", ")", ";", ","};
vector<string> punctuations (punctArr, punctArr + sizeof(punctArr) / sizeof(punctArr[0]));

//TODO: %
const string opArr[] = {"+", "-", "*", "<", ">", "&", ".", "@", "/", ":", "=", "~", "|", "$", "!", "#", "^", "_", "?"};
vector<string> operators (opArr, opArr + sizeof(opArr) / sizeof(opArr[0]));

//constant used for checking if we want to check the next token is a number
const string NUMBER = "number";

const string STRING = "string";

//main function to check if the next token is the same as the supplied argument
bool isNextToken(string input) {
	ignoreCommentsAndSpaces();

	if(contains(identifiers, input)) {
		return isIdentifier(input);
	} else if(contains(punctuations, input)) {
		return isPunctuation(input);
	} else if(input == NUMBER) {
		return isNumber();
	} else if(contains(operators, string(1,input[0]))) {
		return isOperator(input);
	} else if(input == STRING) {
		return isString();
	}
	return false;
}

//helper function to check if next token is an identifier
bool isIdentifier(string input) {
	char ch;
	string token;

	if(isalpha(in.peek())) {
		while(isalpha(in.peek()) || isdigit(in.peek()) || in.peek() == '_'){
        	token += in.get();
    	}

    	if(token == input) {
    		return true;
    	} else {
    		revert(token);
    	}
	}
    return false;
}

//helper function to check if next token is a punctuation
bool isPunctuation(string input) {
	char ch = in.get();
	if(ch == input[0]){
		return true;
	} else {
		in.putback(ch);
		return false;
	}
}

//helper function to check if next token is a number
bool isNumber() {
	bool ret = false;
	while(isdigit(in.peek())) {
		in.get();
		ret = true;
	}
	return ret;
}

//helper function to check if next token is an operator
bool isOperator(string input) {
	string result;
	char ch;

	while(contains(operators, string(1, in.peek()))) {
		ch = in.get();
		result = result + ch;
	}

	if(result == input) {
		return true;
	} else {
		return false;
	}
}

//helper function to check if next token is a string
bool isString() {
	char ch;
	string token;

	if(in.peek() == '\"') {
		ch = in.get();
		token = token + ch;
		while(in.peek() != '\"') {
			ch = in.get();
			token = token + ch;
			if(isEscapedQuotes()) {
				ch = in.get(); //ch here will be the second double quote(thus skipping over it), of an escaped double quote.
				token = token + ch + ch;
			}
			if(in.eof()) {
				return false;
			}
		}
		ch = in.get();
		token = token + ch;
		return true;
	} else {
		return false;
	}
}

bool isEscapedQuotes() {
	char ch;
	if(in.peek() != '\"') {
		return false;
	} else {
		ch = in.get();
		if(in.peek() != '\"') {
			in.putback(ch);
			return false;
		} else {
			return true;
		}
	}
}

void ignoreCommentsAndSpaces() {
	string line;
	ignoreSpace();
	while(isCommentBegin()) {
		//go to end of line;
		getline(in, line);
		cout << "ignoring " << line << "\n";
		ignoreSpace();
	}
}

bool isCommentBegin() {
	string commentBegin;
	char ch = in.get();
	commentBegin = commentBegin + ch;
	ch = in.get();
	commentBegin = commentBegin + ch;

	if(commentBegin == "//") {
		return true;
	} else {
		revert(commentBegin);
		return false;
	}
}

void ignoreSpace() {
	//removes spaces
	while(isspace(in.peek())) {
		in.get();
	}
}

// Helper function - If the string read from input does not match what we're looking for, then this helper function
// can be used to take reset the input file stream head pointer.
void revert(string input) {
	int length = input.size();
    const char* ch = input.c_str();
    for(int i = length - 1; i >= 0; i--){
        in.putback(ch[i]);
    }
}

// Helper function - check if the given input exists within input vector
bool contains(vector<string> vec, string input) {
	if(find(vec.begin(), vec.end(), input) != vec.end()) {
		return true;
	} else {
		return false;
	}
}

int main(int argc, char** argv) {
	string file;
	file = argv[1];
    in.open(file.c_str());

    string check = STRING;
    if(isNextToken(check)){
    	cout << "is " << check << "\n";
    } else {
    	cout << "not " << check << "\n";
    }

    if(isNextToken(check)){
    	cout << "is " << check << "\n";
    } else {
    	cout << "not " << check << "\n";
    }

    in.close();
}