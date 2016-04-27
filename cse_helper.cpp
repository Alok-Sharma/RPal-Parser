//cse_helper.cpp
#include "cse_machine.h"
#include <cstring>

using namespace std;


const string binaryops[] = {"+", "-", "*", "/", "<", "ls", "<=", "le", ">", "gr", ">=", "ge", "|", "aug", "or", "eq", "ne"};
vector<string> cse_binaryops (binaryops, binaryops + sizeof(binaryops) / sizeof(binaryops[0]));

const string unaryops[] = {"not", "!", "**", "neg"};
vector<string> cse_unaryops (unaryops, unaryops + sizeof(unaryops) / sizeof(unaryops[0]));

void startCseMachine(stack<cseNode*> &controlStack, stack<cseNode*> &programStack, map<string, cseNode>* environs[], queue<cseNode*>* deltas[]) {
	cseNode* csenode;
	while(!controlStack.empty()) {
		csenode = controlStack.top();
		string nodename = csenode->name;
		string nodetype = csenode->type;

		if(isInt(nodename)) {
			programStack.push(csenode);
			controlStack.pop();
		} else if(nodename == "lambda") {    // ------ RULE 2 ------//
			programStack.push(csenode);
			controlStack.pop();
		} else if(contains(cse_binaryops, nodename)) {
			cout << "found " << nodename << "\n";
			controlStack.pop(); //pop the binary op

			string s_int1 = programStack.top()->name; //extract the two integers you want to execute the binary op on.
			programStack.pop();
			string s_int2 = programStack.top()->name;
			programStack.pop();
			cseNode* resultNode = executeBinaryOps(nodename, s_int1, s_int2);
			programStack.push(resultNode);
		} 
		else if(nodename == "gamma" && programStack.top()->name == "lambda") {
			controlStack.pop(); // pop the gamma
			cseNode* newEnv = createNextEnvironment(environs); //create a new environment
			controlStack.push(newEnv); //push the new env node to control

			cseNode* lambda = programStack.top(); //get the lambda from the program stack
			int i = lambda->i; //get lambdas i number (delta number)
			string x = extractId(lambda->x); //get lambdas x string
			programStack.pop(); //pop the lambda

			cseNode* valueNode = programStack.top(); //pop the next token from the program stack, add it as a value to the x in the new environs map
			map<string, cseNode> *currentEnv = environs[environCount - 1];
			(*currentEnv)[x] = *valueNode;
			programStack.pop(); //pop the value
			programStack.push(newEnv); //push the new env node to the program

			loadDelta(controlStack, i, deltas); //get the delta queue based on the delta number and load on to program stack.

		} 
		else if(isId(nodename)) {
			string id = extractId(nodename);
			controlStack.pop(); //pop the ID node from control
			map<string, cseNode> *currentEnv = environs[environCount - 1];
			cseNode* idValue = getIdValue(currentEnv, id);
			programStack.push(idValue); //push the ID value to the program
		} else if(nodetype == "e") {
			controlStack.pop(); //pop the e from control
			environStack.pop(); //pop from env stack

			cseNode* popVal = programStack.top();
			programStack.pop(); //pop the top value from the prog stack
			programStack.pop(); //pop the e from prog stack
			programStack.push(popVal); //push back the top val
		}
		 else { // TODO: REMOVE THIS. POPPING OFF UNHANDLES TOKENS
			// controlStack.pop();
		}
		cout << "----\ncontrol:\n";
		printStack(controlStack);
		cout << "\nprogram:\n";
		printStack(programStack);
	}

	//print the final output
	cout << "\nAnswer: " << extractInt(programStack.top()->name) << "\n";
}

bool isInt(string nodename) {
	return nodename.find("<INT:") != std::string::npos;
}

bool isId(string nodename) {
	return nodename.find("<ID:") != std::string::npos;
}

int extractInt(string input) {
	int result = atoi((input).substr(5, input.length() - 6).c_str());
	return result;
}

string extractId(string input) {
	string result = input.substr(4, input.length() - 5);
	return result;
}

cseNode* getIdValue(map<string, cseNode> *env, string key) {
	map<string, cseNode>::iterator iter;
	iter = (*env).find(key);
	if(iter != (*env).end()) {
		return &(iter->second);
	} else {
		cout << "Error, did not find ID in environment.";
		return NULL;
	}
}

cseNode* executeBinaryOps(string op, string s_int1, string s_int2) {
	// s_int1 op s_int2
	int int1 = extractInt(s_int1); //extract the two numbers
	int int2 = extractInt(s_int2);
	cseNode* resultNode;

	if(op == "+") {
		int result = int1 + int2;
		resultNode = createCseNode("INT", "<INT:" + patch::to_string(result) + ">");
	}
	return resultNode;
}