//cse machine.h
#ifndef CSEMACHINE_H
#define CSEMACHINE_H

#include "lexer.h"
#include <queue>
#include <iostream>

using namespace std;

typedef struct cseNodeType{
	string name;
	int i;
	int k;
	string x;
} cseNode;

void evaluate(Node*);
void generateControlStructures(Node*);
void controlStructureHelper (Node*, queue<cseNode*>&, int&, Node* roots[]);
void preorder(queue<Node*>&, Node*);
int countLambda(Node*); 

#endif
