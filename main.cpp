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
const string punctArr[] = {"+", "-", "*", "<", ">", "&", ".", "@", "/", ":", "=", "~", "|", "$", "!", "#", "^", "_", "?"};
vector<string> operators (opArr, opArr + sizeof(opArr) / sizeof(opArr[0]));

//constant used for checking if we want to check the next token is a number
const string NUMBER = "number";

//main function to check if the next token is the same as the supplied argument
bool isNextToken(string input) {
	if(find(identifiers.begin(), identifiers.end(), input) != identifiers.end()) {
		return isIdentifier(input);
	} else if(find(punctuations.begin(), punctuations.end(), input) != punctuations.end()) {
		return isPunctuation(input);
	} else if(input == NUMBER) {
		return isNumber();
	}
	return false;
}

//helper function to check if next token is an identifier
bool isIdentifier(string input) {
	char ch;
	string token;
	//removes spaces
	while(isspace(in.peek())) {
		in.get();
	}

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
	//removes spaces
	while(isspace(in.peek())) {
		in.get();
	}

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
	//removes spaces
	while(isspace(in.peek())) {
		in.get();
	}

	bool ret = false;
	while(isdigit(in.peek())) {
		in.get();
		ret = true;
	}
	return ret;
}

//helper function to check if next token is an operator
bool isOperator(string input) {
	
}

//If the string read from input does not match what we're looking for, then this helper function
// can be used to take reset the input file stream head pointer.
void revert(string input) {
	int length = input.size();
    const char* ch = input.c_str();
    for(int i = length - 1; i >= 0; i--){
        in.putback(ch[i]);
    }
}

int main(int argc, char** argv) {
	string file;
	file = argv[1];
    in.open(file.c_str());

    string check = NUMBER;
    if(isNextToken(check)){
    	cout << "is " << check << "\n";
    } else {
    	cout << "not " << check << "\n";
    }
}