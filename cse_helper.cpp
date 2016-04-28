//cse_helper.cpp
#include "cse_machine.h"
#include <cstring>

using namespace std;


const string binaryops[] = {"+", "-", "*", "/", "<", "ls", "<=", "le", ">", "gr", ">=", "ge", "|", "and", "or", "eq", "ne", "**", "aug"};
vector<string> cse_binaryops (binaryops, binaryops + sizeof(binaryops) / sizeof(binaryops[0]));

const string unaryops[] = {"not", "neg"};
vector<string> cse_unaryops (unaryops, unaryops + sizeof(unaryops) / sizeof(unaryops[0]));

void startCseMachine(stack<cseNode*> &controlStack, stack<cseNode*> &programStack, map<string, cseNode>* environs[], queue<cseNode*>* deltas[], int numOfDelta) {
	cseNode* csenode;
	cseNode* lambdas[numOfDelta];

	while(!controlStack.empty()) {
		csenode = controlStack.top();
		string nodename = csenode->name;
		string nodetype = csenode->type;

		if(isInt(nodename) || nodename == "<true>" || nodename == "<false>" || nodetype == "function" || isStr(nodename) || nodename == "<Y*>") {
			programStack.push(csenode);
			controlStack.pop();

		} else if(nodename == "lambda") {
			csenode->k = environStack.top(); //update the parent env.
			programStack.push(csenode);
			lambdas[csenode->i] = csenode;
			controlStack.pop();

		} else if(contains(cse_binaryops, nodename)) {
			controlStack.pop(); //pop the binary op

			string s_int1 = programStack.top()->name; //extract the two integers you want to execute the binary op on.
			programStack.pop();
			string s_int2 = programStack.top()->name;
			programStack.pop();
			cseNode* resultNode = executeBinaryOps(nodename, s_int1, s_int2);
			programStack.push(resultNode);

		} else if(contains(cse_unaryops, nodename)) {
			controlStack.pop(); //pop the unary
			string input = programStack.top()->name;
			programStack.pop();
			cseNode* resultNode = executeUnaryOps(nodename, input);
			programStack.push(resultNode);

		} else if(nodename == "gamma" && programStack.top()->name == "lambda") {
			controlStack.pop(); // pop the gamma
			cseNode* newEnv = createNextEnvironment(environs, programStack.top()->k); //create a new environment
			controlStack.push(newEnv); //push the new env node to control

			cseNode* lambda = programStack.top(); //get the lambda from the program stack
			// cout << "lambda (i)" << lambda->i << " (x)" << lambda->x << " (k)" << lambda->k << "\n";
			
			int i = lambda->i; //get lambdas i number (delta number)
			programStack.pop(); //pop the lambda
			cseNode* valueNode = programStack.top(); //pop the next token from the program stack, add it as a value to the x in the new environs map
			map<string, cseNode> *currentEnv = environs[environStack.top()];

			//TODO: SUPER UGLY. TAKE CONTENTS OF THIS IF INTO FUNCTION.
			if(valueNode->type == "TUPLE" && lambda->type == "n-ary") {
				string vtuple = valueNode->name;
				vtuple = vtuple.substr(1, vtuple.length() - 2);
				vector<string> valTuple;
				stringstream ss1(vtuple);

				//extract the values
				while(ss1.good()) {
					string substr;
					getline(ss1, substr, ',');
					valTuple.push_back(substr);
				}

				//extract the keys
				string ktuple = lambda->x;
				vector<string> keyTuple;
				stringstream ss2(ktuple);
				while(ss2.good()) {
					string substr;
					getline(ss2, substr, ',');
					keyTuple.push_back(substr);
				}

				for(int i = 0; i < valTuple.size(); i++) {
					string value = valTuple[i];
					value.erase(value.begin(), std::find_if(value.begin(), value.end(), std::bind1st(std::not_equal_to<char>(), ' '))); //remove trailing spaces
					(*currentEnv)[extractId(keyTuple[i])] = *createCseNode("", value);
				}

			} else {
				(*currentEnv)[extractId(lambda->x)] = *valueNode;
			}

			programStack.pop(); //pop the value
			programStack.push(newEnv); //push the new env node to the program

			loadDelta(controlStack, i, deltas); //get the delta queue based on the delta number and load on to program stack.

		} else if(isId(nodename)) {
			string id = extractId(nodename);
			controlStack.pop(); //pop the ID node from control
			map<string, cseNode> *currentEnv = environs[environStack.top()];
			cseNode* idValue = getIdValue(currentEnv, id);
			programStack.push(idValue); //push the ID value to the program

		} else if(nodename == "beta") {
			controlStack.pop(); //pop the beta
			string b = programStack.top()->name;
			programStack.pop(); //pop the boolean

			if(b == "<false>") {
				cseNode* elseDelta = controlStack.top();
				int deltaIndex = elseDelta->i;
				controlStack.pop();
				controlStack.pop();

				loadDelta(controlStack, deltaIndex, deltas);
			} else if(b == "<true>") {
				controlStack.pop(); //get rid of else delta
				cseNode* thenDelta = controlStack.top();
				int deltaIndex = thenDelta->i;
				controlStack.pop(); //get rid of the if delta

				loadDelta(controlStack, deltaIndex, deltas);
			}

		} else if(nodename == "delta") {
			int deltaIndex = csenode->i;
			controlStack.pop();
			loadDelta(controlStack, deltaIndex, deltas);

		} else if(nodename == "tau") {
			int tauSize = csenode->i;
			controlStack.pop(); //pop the tau
			cseNode* result = createTuple(tauSize, programStack);
			programStack.push(result);

		} else if(nodename == "gamma" && programStack.top()->type == "TUPLE") {
			controlStack.pop(); //pop gamma;
			string tuple = programStack.top()->name;
			programStack.pop(); //pop the tuple
			int tupleIndex = extractInt(programStack.top()->name);
			programStack.pop(); //pop the int

			cseNode* result = getTupleIndex(tuple, tupleIndex);
			programStack.push(result);

		} else if(nodename == "gamma" && programStack.top()->name == "<Y*>") {
			controlStack.pop(); //pop the gamma
			programStack.pop(); //pop the ystar
			cseNode* lambda = programStack.top();
			cseNode* eta = createCseNode("ETA", "eta");

			eta->i = lambda->i;
			eta->x = lambda->x;
			eta->k = lambda->k;

			programStack.pop(); //pop the lambda
			programStack.push(eta); //push the eta;
			// cout << "pushed eta (i)" << eta->i << " (x)" << eta->x << " (k)" << eta->k << "\n";

		} else if(nodename == "gamma" && programStack.top()->type == "ETA") {
			cseNode* gamma = createCseNode("gamma", "gamma");
			controlStack.push(gamma); //push another gamma onto control stack

			cseNode* eta = programStack.top();
			int lambdaIndex = eta->i;
			cseNode* lambda = lambdas[lambdaIndex];
			programStack.push(lambda);

		} else if(nodetype == "e") {
			controlStack.pop(); //pop the e from control
			environStack.pop(); //pop from env stack

			cseNode* popVal = programStack.top();
			programStack.pop(); //pop the top value from the prog stack
			programStack.pop(); //pop the e from prog stack
			programStack.push(popVal); //push back the top val

		} else if(nodename == "gamma" && programStack.top()->name == "<ID:Stem>") {
			controlStack.pop(); //pop the gamma
			programStack.pop(); //pop the stem
			string result = programStack.top()->name;
			programStack.pop(); //pop the thing to be sterned

			cseNode* node;
			if(isStr(result)) {
				result = extractStr(result);
				node = createCseNode("", "<STR:'" + result.substr(0,1) + "'>");
			} else if(isInt(result)) {
				result = extractInt(result);
				node = createCseNode("", "<INT:" + result.substr(0,1) + ">");
			}
			programStack.push(node);

		} else if(nodename == "gamma" && programStack.top()->name == "<ID:Conc>") {
			controlStack.pop();
			controlStack.pop(); //pop the two gammas
			programStack.pop(); //pop the conc

			string st1 = programStack.top()->name;
			st1 = extractStr(st1);
			programStack.pop();
			string st2 = programStack.top()->name;
			st2 = extractStr(st2);

			string result = st1 + st2;
			cseNode* node = createCseNode("", "<STR:'" + result + "'>");
			programStack.push(node);

		} else if(nodename == "gamma" && programStack.top()->name == "<ID:Print>") {
			controlStack.pop();
			programStack.pop();

			mPrint(programStack.top());
			programStack.pop();
			cseNode* dummy = createCseNode("DUMMY", "<dummy>");
			programStack.push(dummy);
		} else { // TODO: REMOVE THIS. POPPING OFF UNHANDLES TOKENS
			controlStack.pop();
		}
		// cout << "----\ncontrol:\n";
		// printStack(controlStack);
		// cout << "\nprogram:\n";
		// printStack(programStack);
	}
	cout << "\n";
	//print the final output
	// cout << "\nAnswer: " << extractInt(programStack.top()->name) << "\n";
}

void mPrint(cseNode* node) {
	string nodename = node->name;
	//handle printing a tuple first.
	if(nodename[0] == '(' && nodename[nodename.length() - 1] == ')') {
		string commaValues = nodename.substr(1, nodename.length() - 2); //strip the brackets
		vector<string> result;
		stringstream ss(commaValues);

		while(ss.good()) {
			string value;
			getline(ss, value, ',');
			value.erase(value.begin(), std::find_if(value.begin(), value.end(), std::bind1st(std::not_equal_to<char>(), ' '))); //remove trailing spaces
			result.push_back(value);
		}

		string output = "(";
		for(int i = 0; i < result.size(); i++) {
			int t = extractInt(result[i]);  //TODO: ASSUMING TUPLES HAS INTEGERS ONLY
			if(i >= 1) {
				output = output + ", ";
			}
			output = output + patch::to_string(t);
		}

		cout << output << ")";
	} else if(isId(nodename)) {
		cout << extractId(nodename);
	} else if(isStr(nodename)) {
		cout << extractStr(nodename);
	} else if(isInt(nodename)) {
		cout << extractInt(nodename);
	} else if(nodename == "<true>" || nodename == "<false>") {
		cout << nodename.substr(1, nodename.length() - 2);
	} else if(nodename == "lambda") {
		cout << "[lambda closure: " << extractId(node->x) << ": " << node->i << "]";
	}
}

bool isInt(string nodename) {
	return nodename.find("<INT:") != std::string::npos;
}

bool isStr(string nodename) {
	return nodename.find("<STR:") != std::string::npos;	
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

string extractStr(string input) {
	string result = input.substr(6, input.length() - 8);
	return result;
}

cseNode* createTuple(int n, stack<cseNode*> &programStack) {

	string tuple = "(";
		for(int i = 0; i < n; i ++) {
			if(i >= 1) {
				tuple = tuple + ", ";
			}
			tuple = tuple + programStack.top()->name;
			programStack.pop();
		}
		tuple = tuple + ")";
cseNode* result = createCseNode("TUPLE", tuple);
return result;
}

cseNode* getTupleIndex(string tuple, int index) {
	string commaValues = tuple.substr(1, tuple.length() - 2); //strip the brackets
	// cout << "comma vals: " << commaValues << "\n";
	vector<string> result;
	stringstream ss(commaValues);
	
	while(ss.good()) {
		string substr;
		getline(ss, substr, ',');
		result.push_back(substr);
	}

	string value = result[index - 1]; //rpal indices start from 1
	value.erase(value.begin(), std::find_if(value.begin(), value.end(), std::bind1st(std::not_equal_to<char>(), ' '))); //remove trailing spaces
	cseNode* resultNode = createCseNode("", value);
	return resultNode;
}

//from the input environment, get the value of the input key
cseNode* getIdValue(map<string, cseNode> *env, string key) {
	map<string, cseNode>::iterator iter;
	iter = (*env).find(key);
	if(iter != (*env).end()) {
		return &(iter->second);
	} else {
		cout << "Error, did not find the variable '" << key << "' in environment." << "\n";
		return NULL;
	}
}

cseNode* executeBinaryOps(string op, string s_int1, string s_int2) {
	// s_int1 op s_int2
	cseNode* resultNode;

	if(op == "+") {
		int result = extractInt(s_int1) + extractInt(s_int2);
		resultNode = createCseNode("INT", "<INT:" + patch::to_string(result) + ">");

	} else if(op == "-") {
		int result = extractInt(s_int1) - extractInt(s_int2);
		resultNode = createCseNode("INT", "<INT:" + patch::to_string(result) + ">");

	} else if(op == "/") {
		int result = extractInt(s_int1) / extractInt(s_int2);
		resultNode = createCseNode("INT", "<INT:" + patch::to_string(result) + ">");

	} else if(op == "*") {
		int result = extractInt(s_int1) * extractInt(s_int2);
		resultNode = createCseNode("INT", "<INT:" + patch::to_string(result) + ">");

	} else if(op == "**") {
		int result = pow(extractInt(s_int1), extractInt(s_int2));
		resultNode = createCseNode("INT", "<INT:" + patch::to_string(result) + ">");

	} else if(op == "<" || op == "ls") {
		bool bresult = extractInt(s_int1) < extractInt(s_int2);
		string result;
		if (bresult) {
			result = "true";
		} else {
			result = "false";
		}
		resultNode = createCseNode("BOOL", "<" + result + ">");

	} else if(op == "<=" || op == "le") {
		bool bresult = extractInt(s_int1) <= extractInt(s_int2);
		string result;
		if (bresult) {
			result = "true";
		} else {
			result = "false";
		}
		resultNode = createCseNode("BOOL", "<" + result + ">");

	} else if(op == ">" || op == "gr") {
		bool bresult = extractInt(s_int1) > extractInt(s_int2);
		string result;
		if (bresult) {
			result = "true";
		} else {
			result = "false";
		}
		resultNode = createCseNode("BOOL", "<" + result + ">");

	} else if(op == ">=" || op == "ge") {
		bool bresult = extractInt(s_int1) >= extractInt(s_int2);
		string result;
		if (bresult) {
			result = "true";
		} else {
			result = "false";
		}
		resultNode = createCseNode("BOOL", "<" + result + ">");

	} else if(op == "eq") {
		bool bresult = (extractInt(s_int1) == extractInt(s_int2));
		string result;
		if (bresult) {
			result = "true";
		} else {
			result = "false";
		}
		resultNode = createCseNode("BOOL", "<" + result + ">");

	} else if(op == "nq") {
		bool bresult = (extractInt(s_int1) != extractInt(s_int2));
		string result;
		if (bresult) {
			result = "true";
		} else {
			result = "false";
		}
		resultNode = createCseNode("BOOL", "<" + result + ">");

	} else if(op == "or" || op == "|") {
		if(s_int1 == "<false>" && s_int2 == "<false>") {
			resultNode = createCseNode("BOOL", "<false>");
		} else {
			resultNode = createCseNode("BOOL", "<true>");	
		}

	} else if(op == "and") {
		if(s_int1 == "<true>" && s_int2 == "<true>") {
			resultNode = createCseNode("BOOL", "<true>");
		} else {
			resultNode = createCseNode("BOOL", "<false>");	
		}

	} else if(op == "aug") {
		string toInsert = ", " + s_int2;
		s_int1.insert(s_int1.length() - 1, toInsert);
		resultNode = createCseNode("TUPLE", s_int1);
	}
	return resultNode;
}

cseNode* executeUnaryOps(string op, string input) {
	cseNode* resultNode;

	//BUG: NEG CRASHES.
	if(op == "neg") {
		int int1 = extractInt(input);
		int result = -int1;
		resultNode = createCseNode("INT", "<INT:" + patch::to_string(result) + ">");
	} else if(op == "not") {
		if(input == "<true>") {
			resultNode = createCseNode("BOOL", "<false>");
		} else if(input == "<false>") {
			resultNode = createCseNode("BOOL", "<true>");
		} else {
			cout << "Error, attempt to apply boolean operator on non-boolean expression." << "\n";
		}
	}
	return resultNode;
}