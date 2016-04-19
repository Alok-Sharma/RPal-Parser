//standardizer.
#ifndef STANDARDIZER_H
#define STANDARDIZER_H
#include "lexer.h"
using namespace std;

Node* Standardize(Node*);
Node* StandardizeLet(Node*);
Node* StandardizeWhere(Node*);
Node* StandardizeWithin(Node*);
Node* StandardizeFunction(Node*);
Node* StandardizeAnd(Node*);
Node* StandardizeAt(Node*);
Node* StandardizeRec(Node*);
Node* NewNode(string, int);

#endif
