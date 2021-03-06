//main.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include "parser.h"
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
const string opArr[] = {"+", "-", "*", "<", ">", "&", ".", "@", "/", ":", "=", "|"};
vector<string> operators (opArr, opArr + sizeof(opArr) / sizeof(opArr[0]));

//constant used for checking if we want to check the next token is a number
const string NUMBER = "number";

const string STRING = "string";

const string IDENTIFIER = "identifier";

// Main function to check if the next token is the same as the supplied argument
string isNextToken(string input) {
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
	} else if(input == IDENTIFIER) {
		return isIdentifier(IDENTIFIER);
	}
	return "";
}

// Main function to read the input string
void read(string input) {
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

// Function to read the next IDENTIFIER. This expression expects the next token to be an identifier
// If it isnt, then an error will be thrown.
void readIdentifier() {
	string token = isNextToken(IDENTIFIER);
	if(token != "") {
		//found the identifier
		read(token);
		BuildTree("<ID:" + token + ">", 0);
	} else {
		cout << "Error, expected IDENTIFIER\n";
	}
}

// Function to read the next NUMBER. This expression expects the next token to be a NUMBER
// If it isnt, then an error will be thrown.
void readNumber() {
	string token = isNextToken(NUMBER);
	if(token != "") {
		//found the number
		read(token);
		BuildTree("<INT:" + token + ">", 0);
	} else {
		cout << "Error, expected NUMBER\n";
	}
}

// Function to read the next STRING. This expression expects the next token to be an STRING
// If it isnt, then an error will be thrown.
void readString() {
	string token = isNextToken(STRING);
	if(token != "") {
		//found string
		read(token);
		BuildTree("<STR:" + token +">", 0);
	} else {
		cout << "Error, expected STRING\n";
	}
}

// Function to check if next token is an identifier
string isIdentifier(string input) {
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
string isPunctuation(string input) {
	char ch = in.peek();
	if(ch == input[0]){
		return input;
	} else {
		return "";
	}
}

//helper function to check if next token is a number
string isNumber() {
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
string isOperator(string input) {
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
string isString() {
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
bool isEscapedQuotes() {
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

// main function to ignore any space and comments starting from the current position
// in the file, till we encounter anything apart from a comment/space.
void ignoreCommentsAndSpaces() {
	string line;
	ignoreSpace();
	while(isCommentBegin()) {
		//go to end of line;
		getline(in, line);
		// cout << "ignoring " << line << "\n";
		ignoreSpace();
	}
}

// checks if we are currently at the beginning of a comment.
bool isCommentBegin() {
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

// ignores spaces starting from cureent position, till we encounter anything apart from a space.
void ignoreSpace() {
	//removes spaces
	while(isspace(in.peek())) {
		in.get();
	}
}

// Helper function - If the string read from input does not match what we're looking for, then this helper function
// can be used to take reset the input file stream head pointer.
void revert(string input) {
	int length = input.length();
	// cout << "reverting " << input << " length " << length << "\n";
	if(in.fail()) {
		in.clear();
		reachedEof = true;
		// cout << "failed, fixed";
	}
    for(int i = length - 1; i >= 0; i--){
        in.putback(input[i]);
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