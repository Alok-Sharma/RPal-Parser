//cse machine
#include "cse_machine.h"
#include <iostream>
#include <queue>

using namespace std;

void evaluate(Node* root) {
	generateControlStructures(root);
}

void generateControlStructures(Node* node) {
	int numOfLambda = countLambda(node);
	Node *roots[numOfLambda + 1];
	roots[0] = node;
	queue<cseNode*> *deltas[numOfLambda + 1];
	int count = 0;
	
	for(int i = 0; i < numOfLambda + 1; i++) {
		deltas[i] = new queue<cseNode*>;
		controlStructureHelper(roots[i], *deltas[i], count, roots);	
	}

	for(int j = 0; j < numOfLambda + 1; j++) {
		queue<cseNode*> q = *deltas[j];
		cout << "D" << j << ":\n";
		while(!q.empty()) {
			cout << q.front()->name << "\n";
			q.pop();
		}
		cout << "\n";
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

