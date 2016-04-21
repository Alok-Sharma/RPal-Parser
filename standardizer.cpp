//Standardizer
#include "standardizer.h"
#include <iostream>
#include <fstream>

using namespace std;

Node* Standardize(Node *node) {
	Node* result;
	string name = node->name;

	if(name == "let") {
		result = StandardizeLet(node);
	} else if(name == "where") {
		result = StandardizeWhere(node);
	} else if(name == "within") {
		result = StandardizeWithin(node);
	} else if(name == "function_form") {
		result = StandardizeFunction(node);
	} else if(name == "and") {
		result = StandardizeAnd(node);
	} else if(name == "@") {
		result = StandardizeAt(node);
	} else if(name == "rec") {
		result = StandardizeRec(node);
	} else if(node->count >0) { 
		// if node is none of the above, yet has children
		// then just standardize the children and return node.
		for(int i = 0; i < node->count; i++) {
			node->child[i] = Standardize(node->child[i]);
		}
		result = node;
	} else {
		result = node;
	}
	return result;
}

Node* StandardizeLet(Node *node) {
	node->child[0] = Standardize(node->child[0]);
	node->child[1] = Standardize(node->child[1]);

	node->name = "gamma";
	node->child[0]->name = "lambda";

	Node* E = node->child[0]->child[1];
	Node* P = node->child[1];

	node->child[0]->child[1] = P;
	node->child[1] = E;

	return node;
}

Node* StandardizeWhere(Node *node) {
	node->child[0] = Standardize(node->child[0]);
	node->child[1] = Standardize(node->child[1]);

	node->name = "gamma";
	Node* P = node->child[0];
	Node* X = node->child[1]->child[0];
	Node* E = node->child[1]->child[1];

	Node* lambda = NewNode("lambda", 2);
	node->child[1] = E;
	node->child[0] = lambda;
	node->child[0]->child[0] = X;
	node->child[0]->child[1] = P;

	return node;
}

Node* StandardizeWithin(Node *node) {
	node->child[0] = Standardize(node->child[0]);
	node->child[1] = Standardize(node->child[1]);

	node->name = "=";
	Node* X1 = node->child[0]->child[0]; 
	Node* E1 = node->child[0]->child[1];
	Node* X2 = node->child[1]->child[0];
	Node* E2 = node->child[1]->child[1];

	Node* gamma = NewNode("gamma", 2);
	Node* lambda = NewNode("lambda", 2);

	node->child[0] = X2;
	node->child[1] = gamma;
	node->child[1]->child[0] = lambda;
	node->child[1]->child[1] = E1;
	node->child[1]->child[0]->child[0] = X1;
	node->child[1]->child[0]->child[1] = E2;

	return node;
}

Node* StandardizeFunction(Node *node) {
	node->name = "=";
	int childCount = node->count; //4
	int lambdaCount = childCount - 2; //2
	Node* P = Standardize(node->child[0]);
	Node* E = Standardize(node->child[childCount - 1]);

	Node* temp;
	Node* lambda = NewNode("lambda", 2);
	lambda->child[1] = E;

	for(int i = lambdaCount; i >= 1; i--) {
		lambda->child[0] = Standardize(node->child[i]);
		temp = NewNode("lambda", 2);
		temp->child[1] = lambda;
		lambda = temp;
	}

	//reset the children of the function_form node (now the '=' node)
	node->count = 2;
	node->child = new Node*[2];
	node->child[0] = P;
	node->child[1] = lambda->child[1];

	return node;
}

Node* StandardizeAnd(Node *node) {
	//standardize all immediate children
	for(int i = 0; i < node->count; i++) {
		node->child[i] = Standardize(node->child[i]);
	}

	node->name = "=";
	Node* tau = NewNode("tau", node->count);
	Node* comma = NewNode(",", node->count);
	Node* X;
	Node* E;

	//connect up the children for comma and tau
	for(int j = 0; j < node->count; j++) {
		X = node->child[j]->child[0];
		E = node->child[j]->child[1];
		comma->child[j] = X;
		tau->child[j] = E;
	}

	//connect the new comma and tau nodes as the children for the =
	node->count = 2;
	node->child = new Node*[node->count];
	node->child[0] = comma;
	node->child[1] = tau;

	return node;
}

Node* StandardizeAt(Node *node) {
	node->child[0] = Standardize(node->child[0]);
	node->child[1] = Standardize(node->child[1]);
	node->child[2] = Standardize(node->child[2]);

	Node* E1 = node->child[0];
	Node* N = node->child[1];
	Node* E2 = node->child[2];
	Node* gamma = NewNode("gamma", 2);

	gamma->child[0] = N;
	gamma->child[1] = E1;
	node->name = "gamma";
	node->count = 2;
	node->child = new Node*[node->count];
	node->child[0] = gamma;
	node->child[1] = E2;

	return node;
}

Node* StandardizeRec(Node *node) {
	node->child[0] = Standardize(node->child[0]);
	Node* X = node->child[0]->child[0];
	Node* E = node->child[0]->child[1];

	Node* lambda = NewNode("lambda", 2);
	Node* gamma = NewNode("gamma", 2);
	Node* YStar = NewNode("<Y*>", 0);

	lambda->child[0] = X;
	lambda->child[1] = E;
	gamma->child[0] = YStar;
	gamma->child[1] = lambda;
	node->name = "=";
	node->count = 2;
	node->child = new Node*[node->count];
	node->child[0] = X;
	node->child[1] = gamma;

	return node;
}

//Create a new node with the input name, and a child array with size as input childCount.
Node* NewNode(string name, int childCount) {
	Node* node = new Node;
	node->name = name;
	node->count = childCount;
	node->child = new Node*[childCount];
	return node;
}