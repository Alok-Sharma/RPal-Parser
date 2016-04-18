//standardizer.
#ifndef STANDARDIZER_H
#define STANDARDIZER_H
#include "lexer.h"
using namespace std;

Node* Standardize(Node*);
Node* StandardizeLet(Node*);
Node* StandardizeWhere(Node*);

#endif
