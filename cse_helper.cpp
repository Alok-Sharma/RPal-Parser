//cse_helper.cpp
#include "cse_machine.h"
#include <cstring>

using namespace std;


const string binaryops[] = {"+", "-", "*", "/", "<", "ls", "<=", "le", ">", "gr", ">=", "ge", "|", "aug", "or", "eq", "ne"};
vector<string> cse_binaryops (binaryops, binaryops + sizeof(binaryops) / sizeof(binaryops[0]));

const string unaryops[] = {"not", "!", "**", "neg"};
vector<string> cse_unaryops (unaryops, unaryops + sizeof(unaryops) / sizeof(unaryops[0]));

void startCseMachine(stack<cseNode*> &controlStack, stack<cseNode*> &programStack, map<string,string>* environs[], queue<cseNode*>* deltas[]) {
	cseNode* csenode;
	while(!controlStack.empty()) {
		csenode = controlStack.top();
		string nodename = csenode->name;

		if(isInt(nodename)) {
			programStack.push(csenode);
			controlStack.pop();
		} else if(nodename == "lambda") {    // ------ RULE 2 ------//
			csenode->k = environStack.top(); //give the current environment number to the lambda
			programStack.push(csenode);
			controlStack.pop();
		} else if(contains(cse_binaryops, nodename)) {
			controlStack.pop(); //pop the binary op
			controlStack.pop(); //pop the two gammas following it
			controlStack.pop();

			string s_int1 = programStack.top()->name; //extract the two integers you want to execute the binary op on.
			programStack.pop();
			string s_int2 = programStack.top()->name;
			programStack.pop();
			executeBinaryOps(nodename, s_int1, s_int2);

		} else { // TODO: REMOVE THIS. POPPING OFF UNHANDLES TOKENS
			controlStack.pop();
		}
	}
}

bool isInt(string nodename) {
	if((nodename).substr(0, 4) == "<INT") {
		return true;
	} else {
		return false;
	}
}

bool isId(string nodename) {
	if((nodename).substr(0, 4) == "<ID") {
		return true;
	} else {
		return false;
	}
}

int extractInt(string input) {
	if(isInt(input)) {
		int result = atoi((s_int1).substr(5, nodename.length - 2).c_str());
		return result;
	}
	return NULL;
}

cseNode* executeBinaryOps(string op, string s_int1, string s_int2) {
	// s_int1 op s_int2
	int int1 = extractInt(s_int1); //extract the two numbers
	int int2 = extractInt(s_int2);
	cseNode* result;

	if(op == "+") {
		int result = int1 + int2;
		result = createCseNode("INT", "<INT:" + patch::to_string(result) + ">");
	}
	return result;
}