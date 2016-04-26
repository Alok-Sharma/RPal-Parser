//cse machine
#include "cse_machine.h"
#include <iostream>
#include <queue>
#include <stack>
#include <map>
#include <string>

using namespace std;

//TODO delete everything created using new

void evaluate(Node* root) {
	int numOfLambda = countLambda(root);
	map<string, string> *environs[numOfLambda + 1]; //pointer to an array of maps. Each map is an environment. e0 is at index 0, and so on.
	queue<cseNode*> *deltas[numOfLambda + 1]; // pointer to an array of queues of cseNodes. Each queue is a delta.
	generateControlStructures(root, deltas); // populate the array of queues

	stack<cseNode*> programStack; // The program stack	
	stack<cseNode*> controlStack; // The control stack

	//initilize the control stack;
	controlStack.push(createNextEnvironment(environs)); //push e0;
	loadDelta(controlStack, 0, deltas); //push d0;

	startCseMachine(controlStack, programStack, environs, deltas);

	printControlStack(controlStack);
}

void startCseMachine(stack<cseNode*> &controlStack, stack<cseNode*> &programStack, map<string,string>* environs[], queue<cseNode*>* deltas[]) {
	while(!controlStack.empty()) {

	}
}

void generateControlStructures(Node* node, queue<cseNode*>* deltas[]) {
	int numOfLambda = countLambda(node);
	Node *roots[numOfLambda + 1];
	roots[0] = node;
	int count = 0;
	
	for(int i = 0; i < numOfLambda + 1; i++) {
		deltas[i] = new queue<cseNode*>;
		controlStructureHelper(roots[i], *deltas[i], count, roots);	
	}

	// for(int j = 0; j < numOfLambda + 1; j++) {
	// 	queue<cseNode*> q = *deltas[j];
	// 	cout << "D" << j << ":\n";
	// 	while(!q.empty()) {
	// 		cout << q.front()->name << "\n";
	// 		q.pop();
	// 	}
	// 	cout << "\n";
	// }
}

void controlStructureHelper (Node* node, queue<cseNode*> &q, int &count, Node* roots[]) {
	cseNode* csenode = new cseNode;

	if(node->name == "lambda") {
		csenode->type = "lambda";
		csenode->name = "lambda";
		csenode->i = ++count;
		csenode->x = node->child[0]->name;
		q.push(csenode);
		roots[count] = node->child[1];
	} else {
		csenode->name = node->name;
		q.push(csenode);

		if(node->count > 0) {
			controlStructureHelper(node->child[0], q, count, roots);
			controlStructureHelper(node->child[1], q, count, roots);
		}
	}
}

void preorder(queue<Node*> &q, Node* n) {
	q.push(n);
	if(n->count > 0) {
		preorder(q, n->child[0]);
		preorder(q, n->child[1]);
	}
}

int countLambda(Node* node) {
	queue<Node*> q;
	preorder(q, node);
	int count = 0;
	while(!q.empty()) {
		if(q.front()->name == "lambda") {
			count++;
		}
		q.pop();
	}
	return count;
}

cseNode* createCseNode(string type, string name) {
	cseNode* node = new cseNode;
	node->type = type;
	node->name = name;
	return node;
}

cseNode* createNextEnvironment(map<string, string> *environs[]) {
	cseNode* envNode = createCseNode("e", "e" + patch::to_string(environCount));
	environs[environCount] = new map<string,string>();
	environCount = environCount + 1;
	return envNode;
}

void loadDelta(stack<cseNode*> &controlStack, int deltaNumber, queue<cseNode*>* deltas[]) {
	queue<cseNode*> deltaQueue = *deltas[deltaNumber];
	// cout << "deltas:\n";
	while(!deltaQueue.empty()) {
		// cout << deltaQueue.front()->name << "\n";
		controlStack.push(deltaQueue.front());
		deltaQueue.pop();
	}
}

//printing a copy of the control stack, not the actual one. So dont worry if stuff is popped.
void printControlStack(stack<cseNode*> controlStack) {
	cseNode* node;
	cout << "\ncontrol stack:" << "\n"; 
	while(!controlStack.empty()) {
		node = controlStack.top();
		cout << node->name << " ";
		controlStack.pop();
	}
	cout << "\n";
}

