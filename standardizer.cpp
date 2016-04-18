//Standardizer
#include "standardizer.h"
#include <iostream>
#include <fstream>

using namespace std;

Node* Standardize(Node *node) {
	Node* result;

	if(node->name == "let") {
		result = StandardizeLet(node);
	} else if(node->name == "where") {
		result = StandardizeWhere(node);
	} else {
		result = node;
	}
	return result;
}

Node* StandardizeLet(Node *node) {
	node->name = "gamma";
	node->child[0]->name = "lambda";

	Node* E = Standardize(node->child[0]->child[1]);
	Node* P = Standardize(node->child[1]);

	node->child[0]->child[1] = P;
	node->child[1] = E;
	return node;
}

Node* StandardizeWhere(Node *node) {
	node->name = "gamma";
	Node* P = Standardize(node->child[0]);
	Node* X = Standardize(node->child[1]->child[0]);
	Node* E = Standardize(node->child[1]->child[1]);

	Node* lambda = new Node;
	lambda->name = "lambda";
	lambda->count = 2;
	lambda->child = new Node*[lambda->count];

	node->child[1] = E;
	node->child[0] = lambda;
	node->child[0]->child[0] = X;
	node->child[0]->child[1] = P;
	return node;
}