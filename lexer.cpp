//main.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include "lexer.h"
#include <algorithm>

using namespace std;

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

const string IDENTIFIER = "identifier";
Parser parser;

//main function to check if the next token is the same as the supplied argument
string Lexer::isNextToken(string input) {
	ignoreCommentsAndSpaces();

	if(contains(identifiers, input)) {
		return isIdentifier(input);
		// TODO: buildtree
	} else if(contains(punctuations, input)) {
		return isPunctuation(input);
	} else if(input == NUMBER) {
		return isNumber();
		//TODO: buildtree
	} else if(contains(operators, string(1,input[0]))) {
		return isOperator(input);
	} else if(input == STRING) {
		return isString();
		//TODO: buildtree
	} else if(input == IDENTIFIER) {
		return isIdentifier(IDENTIFIER);
	}
	return "";
}

void Lexer::read(string input) {
	ignoreCommentsAndSpaces();

	char ch;
	string token = "";
	for(int i = 0; i < input.length(); i++) {
		ch =in.get();
		token = token + ch;
	}
	if(token != input) {
		cout << "Error, expected " << input <<"\n";
	} else {
		// cout << input << "\n";
	}
}

void Lexer::readIdentifier() {
	string token = isNextToken(IDENTIFIER);
	if(token != "") {
		//found the identifier
		read(token);
		parser.BuildTree("<ID:" + token + ">", 0);
	} else {
		cout << "Error, expected IDENTIFIER\n";
	}
}

void Lexer::readNumber() {
	string token = isNextToken(NUMBER);
	if(token != "") {
		//found the number
		read(token);
		parser.BuildTree("<INT:" + token + ">", 0);
	} else {
		cout << "Error, expected NUMBER\n";
	}
}

void Lexer::readString() {
	string token = isNextToken(STRING);
	if(token != "") {
		//found string
		read(token);
		parser.BuildTree("<STR:" + token +">", 0);
	} else {
		cout << "Error, expected STRING\n";
	}
}

//helper function to check if next token is an identifier
string Lexer::isIdentifier(string input) {
	char ch;
	string token;

	if(isalpha(in.peek())) {
		while(isalpha(in.peek()) || isdigit(in.peek()) || in.peek() == '_'){
        	token += in.get();
    	}
    	// cout << "is identifier before reverting " << token <<"\n";
    	revert(token);
    	if(token == input) {
    		return token;
    	} else if (input == IDENTIFIER && !contains(identifiers, token)) { //return any identifier, but shouldnt be a key word
    		return token;
    	} else {
    		return "";
    	}
	}
	return "";
}

//helper function to check if next token is a punctuation
string Lexer::isPunctuation(string input) {
	char ch = in.peek();
	if(ch == input[0]){
		return input;
	} else {
		return "";
	}
}

//helper function to check if next token is a number
string Lexer::isNumber() {
	char ch;
	string token = "";

	while(isdigit(in.peek())) {
		ch = in.get();
		token = token + ch;
	}
	revert(token);
	return token;
}

//helper function to check if next token is an operator
string Lexer::isOperator(string input) {
	string result = "";
	char ch;

	while(contains(operators, string(1, in.peek()))) {
		ch = in.get();
		result = result + ch;
	}

	if(result == input) {
		revert(result);
		return result;
	} else if(result != "") {
		revert(result);
	}
	return "";
}

//helper function to check if next token is a string
string Lexer::isString() {
	char ch;
	string token = "";

	if(in.peek() == '\"' || in.peek() == '\'') {
		char quoteType = in.peek();
		ch = in.get();
		token = token + ch;
		while(in.peek() != quoteType) {
			ch = in.get();
			token = token + ch;
			if(isEscapedQuotes()) {
				ch = in.get(); //ch here will be the second double quote(thus skipping over it), of an escaped double quote. 
								//Rpal escapes with double of whatever you want to escape
				token = token + ch + ch;
			}
			if(in.peek() == EOF) {
				revert(token);
				return "";
			}
		}
		ch = in.get();
		token = token + ch;
		revert(token);
		return token;
	} else {
		return "";
	}
}

//helper function to skip overescaped quotes. Used in isString.
bool Lexer::isEscapedQuotes() {
	char ch;
	if(in.peek() != '\"' || in.peek() != '\'') {
		return false;
	} else {
		ch = in.get();
		if(in.peek() != ch) {
			in.putback(ch);
			return false;
		} else {
			return true;
		}
	}
}

void Lexer::ignoreCommentsAndSpaces() {
	string line;
	ignoreSpace();
	while(isCommentBegin()) {
		//go to end of line;
		getline(in, line);
		// cout << "ignoring " << line << "\n";
		ignoreSpace();
	}
}

bool Lexer::isCommentBegin() {
	string commentBegin = "";

	if(in.peek() !=  EOF) {
		// cout << "not end of file\n";
		commentBegin = in.get();
	}
	if(in.peek() != EOF) {
		// cout << "not end of file\n";
		commentBegin += in.get();
	}

	if(commentBegin == "//") {
		return true;
	} else if(commentBegin != "") {
		// cout << "before reverting in comment\n";
		revert(commentBegin);
		return false;
	}
	return false;
}

void Lexer::ignoreSpace() {
	//removes spaces
	while(isspace(in.peek())) {
		in.get();
	}
}

// Helper function - If the string read from input does not match what we're looking for, then this helper function
// can be used to take reset the input file stream head pointer.
void Lexer::revert(string input) {
	int length = input.length();
	// cout << "reverting " << input << " length " << length << "\n";
	if(in.fail()) {
		in.clear();
		reachedEof = true;
	}
    for(int i = length - 1; i >= 0; i--){
        in.putback(input[i]);
    }
}

// Helper function - check if the given input exists within input vector
bool Lexer::contains(vector<string> vec, string input) {
	if(find(vec.begin(), vec.end(), input) != vec.end()) {
		return true;
	} else {
		return false;
	}
}