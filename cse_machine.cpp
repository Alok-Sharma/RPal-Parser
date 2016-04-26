//cse machine
#include "cse_machine.h"

using namespace std;

//TODO delete everything created using new

void evaluate(Node* root) {
	int numOfLambda = countLambda(root);
	map<string, string> *environs[numOfLambda + 1]; //pointer to an array of maps. Each map is an environment. e0 is at index 0, and so on.
	queue<cseNode*> *deltas[numOfLambda + 1]; // pointer to an array of queues of cseNodes. Each queue is a delta.
	generateControlStructures(root, deltas); // populate the array of queues

	stack<cseNode*> programStack; // The program stack	
	stack<cseNode*> controlStack; // The control stack

	//initilize the environment, control stack, and program stack;
	cseNode* n = createNextEnvironment(environs); //create e0;
	controlStack.push(n); //push e0 to control;
	loadDelta(controlStack, 0, deltas); //push d0 to control;
	programStack.push(n); // push e0 to program;

	// startCseMachine(controlStack, programStack, environs, deltas);

	printStack(programStack);


	//------testing------//
	// cseNode* n1 = createNextEnvironment(environs);
	// (*environs[0])["Nikita"] = "pudgu";
	// (*environs[0])["Alok"] = "baby";
	// cseNode* n2 = createNextEnvironment(environs);
	// printMap(environs[1]);
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
}

void controlStructureHelper (Node* node, queue<cseNode*> &q, int &count, Node* roots[]) {
	cseNode* csenode = new cseNode;

	if(node->name == "lambda") {
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

// creates a new environment, and pushes it to the array of environ maps. Also returns a corresponding csenode for that environment.
cseNode* createNextEnvironment(map<string, string> *environs[]) {
	cseNode* envNode = createCseNode("e", "e" + patch::to_string(environCount));
	environs[environCount] = new map<string,string>();
	if(!environStack.empty()) {
		//pop the top environment, and copy everything in its map into newly created map.
		int parentIndex = environStack.top();
		map<string,string> parentEnv = *environs[parentIndex];
		map<string,string> *currentEnv = environs[environCount];
		(*currentEnv).insert(parentEnv.begin(), parentEnv.end()); //hopefully a copy? Copy data from parent environment.
		// cout << "cur env size: " << currentEnv.size() << "\n";
	}
	environStack.push(environCount);
	cout << "env stack top:" << environStack.top() <<"\n";
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
void printStack(stack<cseNode*> instack) {
	cseNode* node;
	cout << "\nstack:" << "\n"; 
	while(!instack.empty()) {
		node = instack.top();
		cout << node->name << " ";
		instack.pop();
	}
	cout << "\n";
}

void printMap(map<string,string> *inmap) {
	typedef map<string,string>::iterator iter;
	cout << (*inmap).size() << "\n";
	for(iter iterator = (*inmap).begin(); iterator != (*inmap).end(); ++iterator) {
		cout << "hi\n";
	}
}