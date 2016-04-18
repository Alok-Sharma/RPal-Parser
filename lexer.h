//lexer.h
#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

//globals
extern ifstream in;
extern const string NUMBER;
extern const string STRING;
extern const string IDENTIFIER;
extern bool reachedEof;

// Node struct for the non-standardized AST
typedef struct nodeType{
	string name;
	struct nodeType** child;
	int count;
} Node;


//lexer util functions
void revert(string);
bool contains(vector<string>, string);
void ignoreSpace();
void ignoreCommentsAndSpaces();
bool isEscapedQuotes();
void read(string);
void readIdentifier();
void readNumber();
void readString();
bool isCommentBegin();

//lexer : check next token functions.
string isNextToken(string);
string isIdentifier(string);
string isPunctuation(string);
string isNumber();
string isOperator(string);
string isString();

#endif
