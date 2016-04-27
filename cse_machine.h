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
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

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


extern int environCount;
extern stack<int> environStack;

//cse_machine functions
void evaluate(Node*);
void generateControlStructures(Node*, queue<cseNode*>*[]);
void controlStructureHelper (Node*, queue<cseNode*>&, int&, Node*[]);
void preorder(queue<Node*>&, Node*);
int countDelta(Node*);
cseNode* createCseNode(string, string);
cseNode* createNextEnvironment(map<string, cseNode>*[], int);
void loadDelta(stack<cseNode*>&, int, queue<cseNode*>*[]);
void printStack(stack<cseNode*>);
void printMap(map<string, cseNode>*);
void printDeltas (int, queue<cseNode*>*[]);

//cse_helper functions
void startCseMachine(stack<cseNode*>&, stack<cseNode*>&, map<string, cseNode>*[], queue<cseNode*>*[]);
bool isInt(string);
bool isStr(string);
bool isId(string);
void mPrint(cseNode*);
cseNode* executeBinaryOps(string, string, string);
cseNode* executeUnaryOps(string, string);
int extractInt(string);
string extractId(string);
string extractStr(string);
cseNode* createTuple(int, stack<cseNode*>&);
cseNode* getIdValue(map<string, cseNode>*, string);
cseNode* getTupleIndex(string, int);

#endif
