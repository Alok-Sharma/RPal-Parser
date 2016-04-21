//parser.cpp

#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "standardizer.h"
#include "cse_machine.h"
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;
ifstream in;
ofstream out;
bool reachedEof;
stack<Node*> nodeStack;

//------------------ Expressions non-terminals ---------------------//
void E() {
	if(isNextToken("let") == "let") {
		read("let");
		D();
		read("in");
		E();
		BuildTree("let", 2);
	} else if(isNextToken("fn") == "fn") {
		read("fn");
		int n = 1;
		Vb();
		while(isNextToken(".") != ".") {
			n++;
			Vb();
		}
		read(".");
		E();
		BuildTree("lambda", n+1);
	} else {
		Ew();
	}
}

void Ew() {
	T();
	if(isNextToken("where") == "where") {
		read("where");
		Dr();
		BuildTree("where", 2);
	}
}

//--------------------- Tuple expressions non-terminals ---------------//
void T() {
	Ta();

	if(isNextToken(",") == ",") {
		int n = 1;
		while(isNextToken(",") == ",") {
			read(",");
			Ta();
			n++;
		}
		BuildTree("tau", n);
	}
}

void Ta() {
	Tc();
	while(isNextToken("aug") == "aug") {
		read("aug");
		Tc();
		BuildTree("aug", 2);
	}
}

void Tc() {
	B();
	if(isNextToken("->") == "->") {
		read("->");
		Tc();
		read("|");
		Tc();
		BuildTree("->", 3);
	}
}

//----------------- Boolean expressions non-terminals -------------//
void B() {
	Bt();
	while(isNextToken("or") == "or") {
		read("or");
		Bt();
		BuildTree("or", 2);
	}
}

void Bt() {
	Bs();
	while(isNextToken("&") == "&") {
		read("&");
		Bs();
		BuildTree("&", 2);
	}
}

void Bs() {
	if(isNextToken("not") == "not") {
		read("not");
		Bp();
		BuildTree("not", 1);
	} else {
		Bp();
	}
}

void Bp() {
	A();

	if(isNextToken("gr") == "gr") {
		read("gr");
		A();
		BuildTree("gr", 2);
	} else if(isNextToken(">") == ">") {
		read(">");
		A();
		BuildTree(">", 2);
	} else if(isNextToken("ge") == "ge") {
		read("ge");
		A();
		BuildTree("ge", 2);
	} else if(isNextToken(">=") == ">=") {
		read(">=");
		A();
		BuildTree(">=", 2);
	} else if(isNextToken("ls") == "ls") {
		read("ls");
		A();
		BuildTree("ls", 2);
	} else if(isNextToken("<") == "<") {
		read("<");
		A();
		BuildTree("<", 2);
	} else if(isNextToken("le") == "le") {
		read("le");
		A();
		BuildTree("le", 2);
	} else if(isNextToken("<=") == "<=") {
		read("<=");
		A();
		BuildTree("<=", 2);
	} else if(isNextToken("eq") == "eq") {
		read("eq");
		A();
		BuildTree("eq", 2);
	} else if(isNextToken("ne") == "ne") {
		read("ne");
		A();
		BuildTree("ne", 2);
	}
}

//---------------- Arithmetic expressions non-terminals --------------//
void A() {
	if(isNextToken("+") == "+") {
		read("+");
		At();
	} else if(isNextToken("-") == "-") {
		read("-");
		At();
		BuildTree("neg", 1);
	} else {
		At();
	}
	
	while(isNextToken("+") == "+" || isNextToken("-") == "-") {
		if(isNextToken("+") == "+") {
			read("+");
			At();
			BuildTree("+", 2);
		} else {
			read("-");
			At();
			BuildTree("-", 2);
		}
	}
}

void At() {
	Af();

	while(isNextToken("*") == "*" || isNextToken("/") == "/") {
		if(isNextToken("*") == "*") {
			read("*");
			Af();
			BuildTree("*", 2);
		} else if(isNextToken("/") == "/") {
			read("/");
			Af();
			BuildTree("/", 2);
		}
	}
}

void Af() {
	Ap();
	if(isNextToken("**") == "**") {
		read("**");
		Af();
		BuildTree("**", 2);
	}
}

void Ap() {
	R();
	while(isNextToken("@") == "@") {
		read("@");
		readIdentifier();
		R();
		BuildTree("@", 3);
	}
}


//------------------- Rators and Rands non terminals ---------------//
void R() {
	Rn();
	while(isNextToken(IDENTIFIER) != ""
		|| isNextToken(NUMBER) != ""
		|| isNextToken(STRING) != "" 
		|| isNextToken("true") == "true"
		|| isNextToken("false") == "false"
		|| isNextToken("nil") == "nil"
		|| isNextToken("dummy") == "dummy"
		|| isNextToken("(") == "(") {
		
		Rn();
		BuildTree("gamma", 2);
	}
}

void Rn() {
	if(isNextToken(IDENTIFIER) != "") {
		readIdentifier();
	} else if(isNextToken(NUMBER) != "") {
		readNumber();
	} else if(isNextToken(STRING) != "") {
		readString();
	} else if(isNextToken("true") == "true") {
		read("true");
		BuildTree("<true>", 0);
	} else if(isNextToken("false") == "false") {
		read("false");
		BuildTree("<false>", 0);
	} else if(isNextToken("nil") == "nil") {
		read("nil");
		BuildTree("<nil>", 0);
	} else if(isNextToken("dummy") == "dummy") {
		read("dummy");
		BuildTree("<dummy>", 0);
	} else if(isNextToken("(") == "(") {
		read("(");
		E();
		read(")");
	} else if(!reachedEof) {
		cout << "Error in Rn.\n";
	} else {
		//reached eof
		//set eof back to false ?
	}
}

//--------------- Definitions non terminals--------------//

void D() {
	Da();
	if(isNextToken("within") == "within") {
		read("within");
		D();
		BuildTree("within", 2);
	}
}

void Da() {
	Dr();
	if(isNextToken("and") == "and") {
		int n =1;
		while(isNextToken("and") == "and") {
			read("and");
			n++;
			Dr();
		}
		BuildTree("and", n);
	}
}

void Dr() {
	if(isNextToken("rec") == "rec") {
		read("rec");
		Db();
		BuildTree("rec", 1);
	} else {
		Db();
	}
	
}

void Db() {
	if(isNextToken("(") == "(") {
		read("(");
		D();
		read(")");
	} else {
		//look two tokens ahead. Depending on the token after the first identifier, decide where to go.
		string nextIdentifier = isNextToken(IDENTIFIER);
		readIdentifier();
		if(isNextToken(IDENTIFIER) != "" || isNextToken("(") == "(") {
			int n = 0;
			while(isNextToken(IDENTIFIER) != "" || isNextToken("(") == "(") {
				Vb();
				n++;
			}
			read("=");
			E();
			BuildTree("function_form", n + 2);
		} else {
			if(isNextToken(",") == ",") {
				//before calling Vl, put back the first identifier you pulled out earlier.
				revert(nextIdentifier);
				Vl();
			}
			read("=");
			E();
			BuildTree("=", 2);
		}
	}
}

//-------------- Variable non terminals ------------//

void Vb() {
	if(isNextToken("(") == "(") {
		read("(");
		if(isNextToken(")") == (")")) {
			read(")");
			//TODO: build tree "()"		
			BuildTree("()", 0);
		} else {
			Vl();
			read(")");
		}

	} else{
		readIdentifier();
	}
}

void Vl() {
	readIdentifier();
	int n = 1;
	if(isNextToken(",") == ",") {
		while(isNextToken(",") == ",") {
			read(",");
			readIdentifier();
			n++;
		}
		BuildTree(",", n);
	}
}

//------------------------ Helper functions -------------------//

//Build tree for the input nodename, and pops the popcount number of trees from the stack, making them its children
void BuildTree(string nodeName, int popCount){
    
    Node* n = new Node;
    n->name = nodeName;
    n->count = popCount;
    n->child = new Node*[popCount];
    
    for(int i = popCount-1; i >= 0; i--){
        Node* temp = nodeStack.top();
        nodeStack.pop();
        n->child[i] = temp;
    }
    nodeStack.push(n);
}

//function to print the tree according to the specified formatting.
void DispTree(Node *node, int level){
    if(node == NULL)
        return;
    for(int i = 1; i <= level; i++)
        cout<<".";
    cout<<node->name<<" "<<endl;

    if(node->child == NULL)
        return;
    
    for(int i = 0; i < node->count; i++){
        DispTree(node->child[i], level+1);
    }
}

int main(int argc, char** argv) {
	bool dispAST = false;
	bool dispST = false;
	reachedEof = false;
	string file;

	for(int count = 1; count < argc; count++) {
		if(string(argv[count]) == "-ast") {
			dispAST = true;
		} else if(string(argv[count]) == "-st") {
			dispST = true;
		} else if(string(argv[count])[0] != '-') {
			file = string(argv[count]);
			// parser.Helper(file);
    		in.open(file.c_str());
		}
	}

	//Parse, Build, and display the AST
    E();
    if(dispAST) {
    	DispTree(nodeStack.top(), 0);
    }

    //Standardize and display ST
    Standardize(nodeStack.top());
    if(dispST) {
    	DispTree(nodeStack.top(), 0);
    }

    evaluate(nodeStack.top());

    in.close();
}                                                                                                                                                                   