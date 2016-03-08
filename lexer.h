//lexer.h
#include <vector>
using namespace std;

//globals
extern std::ifstream in;
extern const std::string NUMBER;
extern const std::string STRING;
extern const std::string IDENTIFIER;

class Lexer {
public:
//lexer util functions
	void revert(std::string);
	bool contains(std::vector<std::string>, std::string);
	void ignoreSpace();
	void ignoreCommentsAndSpaces();
	bool isEscapedQuotes();
	void read(std::string);
	bool isCommentBegin();

//lexer : check next token functions.
	string isNextToken(std::string);
	string isIdentifier(std::string);
	string isPunctuation(std::string);
	string isNumber();
	string isOperator(std::string);
	string isString();
};

class Parser {
public:
	//Parser functions
	void E();
	void D();
	void Db();
	void Vb();
	void Vl();
};