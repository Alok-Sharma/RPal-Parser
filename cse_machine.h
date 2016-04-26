//cse machine.h
#ifndef CSEMACHINE_H
#define CSEMACHINE_H

#include "lexer.h"
#include <queue>
#include <iostream>
#include <map>
#include <stack>
#include <cstdlib>
#include <string>

using namespace std;

typedef struct cseNodeType{
	string type;
	string name;
	int i;
	int k;
	string x;
} cseNode;

#include <string>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


static int environCount = 0;
static stack<int> environStack;

//cse_machine functions
void evaluate(Node*);
void generateControlStructures(Node*, queue<cseNode*>*[]);
void controlStructureHelper (Node*, queue<cseNode*>&, int&, Node*[]);
void preorder(queue<Node*>&, Node*);
int countLambda(Node*);
cseNode* createCseNode(string, string);
cseNode* createNextEnvironment(map<string, string>*[]);
void loadDelta(stack<cseNode*>&, int, queue<cseNode*>*[]);
void printStack(stack<cseNode*>);
void printMap(map<string,string>*);

//cse_helper functions
void startCseMachine(stack<cseNode*>&, stack<cseNode*>&, map<string,string>*[], queue<cseNode*>*[]);
bool isInt(string);
bool isId(string);
cseNode* executeBinaryOps(string, string, string);
int extractInt(string);

#endif
