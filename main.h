//main.h

//util functions
void revert(std::string);
bool contains(std::vector<std::string>, std::string);
void ignoreSpace();
void ignoreCommentsAndSpaces();
bool isEscapedQuotes();

//check next token functions.
bool isNextToken(std::string);
bool isIdentifier(std::string);
bool isPunctuation(std::string);
bool isNumber();
bool isOperator(std::string);
bool isString();
bool isCommentBegin();
