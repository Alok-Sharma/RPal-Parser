//cse machine
#include "cse_machine.h"

using namespace std;

//TODO delete everything created using new
stack<int> environStack;
int environCount = 0;

void evaluate(Node* root) {
	int numOfDelta = countDelta(root);
	map<string, cseNode> *environs[numOfDelta]; //pointer to an array of maps. Each map is an environment. e0 is at index 0, and so on.
	queue<cseNode*> *deltas[numOfDelta]; // pointer to an array of queues of cseNodes. Each queue is a delta.
	generateControlStructures(root, deltas); // populate the array of queues

	stack<cseNode*> programStack; // The program stack	
	stack<cseNode*> controlStack; // The control stack

	//initilize the environment, control stack, and program stack;
	cseNode* n = createNextEnvironment(environs, -1); //create e0 with a negative parent index, indicating this env has no parents.
	controlStack.push(n); //push e0 to control;
	loadDelta(controlStack, 0, deltas); //push d0 to control;
	programStack.push(n); // push e0 to program;

	// printDeltas(numOfDelta, deltas);
	startCseMachine(controlStack, programStack, environs, deltas);

	// printStack(programStack);

	//------testing------//
	// cseNode* n1 = createNextEnvironment(environs);
	// (*environs[0])["Nikita"] = "pudgu";
	// (*environs[0])["Alok"] = "baby";
	// cseNode* n2 = createNextEnvironment(environs);
	// printMap(environs[1]);
}

void generateControlStructures(Node* node, queue<cseNode*>* deltas[]) {
	int numOfDelta = countDelta(node);
	Node *roots[numOfDelta];
	roots[0] = node;
	int count = 0;
	
	for(int i = 0; i < numOfDelta; i++) {
		deltas[i] = new queue<cseNode*>;
		controlStructureHelper(roots[i], *deltas[i], count, roots);	
	}
}

void controlStructureHelper (Node* node, queue<cseNode*> &q, int &count, Node* roots[]) {
	cseNode* csenode = new cseNode;

	if(node->name == "lambda") {
		csenode->name = "lambda";
		csenode->i = ++count;

		if(node->child[0]->name == ",") {
			string result = "";
			for(int i = 0; i < node->child[0]->count; i++) {
				if(i >= 1) {
					result = result +  ",";
				}
				result = result + node->child[0]->child[i]->name;
			}
			csenode->x = result;
		} else {
			csenode->x = node->child[0]->name;	
		}
		
		q.push(csenode);
		roots[count] = node->child[1];

	} else if(node->name == "->") {
		cseNode* delta1 = new cseNode;
		delta1->name = "delta";
		delta1->type = "delta_true";
		delta1->i = ++count;
		roots[count] = node->child[1];
		q.push(delta1);

		cseNode* delta2 = new cseNode;
		delta2->name = "delta";
		delta2->type = "delta_false";
		delta2->i = ++count;
		roots[count] = node->child[2];
		q.push(delta2);

		csenode->name = "beta";
		q.push(csenode);

		controlStructureHelper(node->child[0], q, count, roots);

	} else if(node->name == "tau") {
		csenode->name = "tau";
		csenode->i = node->count;

		q.push(csenode);
		for(int i = 0; i < node->count; i++) {
			roots[++count] = node->child[i];
			cseNode* delta = new cseNode;
			delta->name = "delta";
			delta->type = "tau_delta";
			delta->i = count;
			q.push(delta);
		}

	} else {
		csenode->name = node->name;
		if(node->name == "<ID:Print>") {
			csenode->type = "function";
		}
		q.push(csenode);

		for(int i = 0; i < node->count; i++) {
			controlStructureHelper(node->child[i], q, count, roots);
		}
	}
}

void preorder(queue<Node*> &q, Node* n) {
	q.push(n);
	for(int i = 0; i < n->count; i++) {
		preorder(q, n->child[i]);
	}
}

int countDelta(Node* node) {
	queue<Node*> q;
	preorder(q, node);
	int count = 1;
	while(!q.empty()) {
		if(q.front()->name == "lambda") {
			count++;
		} else if(q.front()->name == "->") {
			count = count + 2;
		} else if(q.front()->name == "tau") {
			count = count + node->count;
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
cseNode* createNextEnvironment(map<string, cseNode> *environs[], int parentIndex) {
	cseNode* envNode = createCseNode("e", "e" + patch::to_string(environCount));
	environs[environCount] = new map<string,cseNode>();

	//root env wont have a parent, will receive -1 as parent index.
	if(parentIndex >= 0) {
		//pop the top environment, and copy everything in its map into newly created map.
		map<string, cseNode> parentEnv = *environs[parentIndex];
		map<string, cseNode> *currentEnv = environs[environCount];
		(*currentEnv).insert(parentEnv.begin(), parentEnv.end()); //hopefully a copy? Copy data from parent environment.
		// cout << "cur env size: " << currentEnv.size() << "\n";
	}
	environStack.push(environCount);
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
	while(!instack.empty()) {
		node = instack.top();
		cout << node->name << " ";
		instack.pop();
	}
	cout << "\n";
}

void printMap(map<string, cseNode> *inmap) {
	typedef map<string, cseNode>::iterator iter;
	cout << (*inmap).size() << "\n";
	for(iter iterator = (*inmap).begin(); iterator != (*inmap).end(); ++iterator) {
		cout << "hi\n";
	}
}

void printDeltas(int lambdas, queue<cseNode*>* deltas[]) {

	cout << "printing deltas : \n";
	for(int j = 0; j < lambdas; j++) {
		queue<cseNode*> qu = *deltas[j];
		cout << "D" << j << ":\n";
		while(!qu.empty()) {
			cout << qu.front()->name << "\n";
			qu.pop();
		}
		cout << "\n";
	}
}