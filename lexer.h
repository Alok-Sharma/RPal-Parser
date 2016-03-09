//lexer.h
#include <vector>
using namespace std;

//globals
extern std::ifstream in;
extern const std::string NUMBER;
extern const std::string STRING;
extern const std::string IDENTIFIER;
extern bool reachedEof;

typedef struct nodeType{
    std::string name;
    struct nodeType** child;
    int count;
} Node;

class Lexer {
public:
//lexer util functions
	void revert(std::string);
	bool contains(std::vector<std::string>, std::string);
	void ignoreSpace();
	void ignoreCommentsAndSpaces();
	bool isEscapedQuotes();
	void read(std::string);
	void readIdentifier();
	void readNumber();
	void readString();
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
	void Test();
	void Helper(string);
	void BuildTree(string name, int count);
	void DispTree(Node *node);
	void DispTree(Node *node, int level);

	//Parser functions
	void E();
	void Ew();

	void T();
	void Ta();
	void Tc();

	void B();
	void Bt();
	void Bs();
	void Bp();

	void A();
	void At();
	void Af();
	void Ap();

	void R();
	void Rn();

	void D();
	void Da();
	void Dr();
	void Db();

	void Vb();
	void Vl();

	void killYourself();
};