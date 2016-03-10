//parser.cpp

#include <iostream>
#include <fstream>
#include "lexer.cpp"
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;
ifstream in;
ofstream out;
Lexer lexer;
bool reachedEof;
stack<Node*> s;

void Parser::E() {
	if(lexer.isNextToken("let") == "let") {
		lexer.read("let");
		D();
		lexer.read("in");
		E();
		BuildTree("let", 2);
	} else if(lexer.isNextToken("fn") == "fn") {
		lexer.read("fn");
		int n = 1;
		Vb();
		while(lexer.isNextToken(".") != ".") {
			n++;
			Vb();
		}
		lexer.read(".");
		E();
		BuildTree("lambda", n+1);
	} else {
		Ew();
	}
}

void Parser::Ew() {
	T();
	if(lexer.isNextToken("where") == "where") {
		lexer.read("where");
		Dr();
		BuildTree("where", 2);
	}
}

void Parser::T() {
	Ta();

	if(lexer.isNextToken(",") == ",") {
		int n = 1;
		while(lexer.isNextToken(",") == ",") {
			lexer.read(",");
			Ta();
			n++;
		}
		BuildTree("tau", n);
	}
}

void Parser::Ta() {
	Tc();
	while(lexer.isNextToken("aug") == "aug") {
		lexer.read("aug");
		Tc();
		BuildTree("aug", 2);
	}
}

void Parser::Tc() {
	B();
	if(lexer.isNextToken("->") == "->") {
		lexer.read("->");
		Tc();
		lexer.read("|");
		Tc();
		BuildTree("->", 3);
	}
}

void Parser::B() {
	Bt();
	while(lexer.isNextToken("or") == "or") {
		lexer.read("or");
		Bt();
		BuildTree("or", 2);
	}
}

void Parser::Bt() {
	Bs();
	while(lexer.isNextToken("&") == "&") {
		lexer.read("&");
		Bs();
		BuildTree("&", 2);
	}
}

void Parser::Bs() {
	if(lexer.isNextToken("not") == "not") {
		lexer.read("not");
		Bp();
		BuildTree("not", 1);
	} else {
		Bp();
	}
}

void Parser::Bp() {
	A();

	if(lexer.isNextToken("gr") == "gr") {
		lexer.read("gr");
		A();
		BuildTree("gr", 2);
	} else if(lexer.isNextToken(">") == ">") {
		lexer.read(">");
		A();
		BuildTree("gr", 2);
	} else if(lexer.isNextToken("ge") == "ge") {
		lexer.read("ge");
		A();
		BuildTree("ge", 2);
	} else if(lexer.isNextToken(">=") == ">=") {
		lexer.read(">=");
		A();
		BuildTree("ge", 2);
	} else if(lexer.isNextToken("ls") == "ls") {
		lexer.read("ls");
		A();
		BuildTree("ls", 2);
	} else if(lexer.isNextToken("<") == "<") {
		lexer.read("<");
		A();
		BuildTree("ls", 2);
	} else if(lexer.isNextToken("le") == "le") {
		lexer.read("le");
		A();
		BuildTree("le", 2);
	} else if(lexer.isNextToken("<=") == "<=") {
		lexer.read("<=");
		A();
		BuildTree("le", 2);
	} else if(lexer.isNextToken("eq") == "eq") {
		lexer.read("eq");
		A();
		BuildTree("eq", 2);
	} else if(lexer.isNextToken("ne") == "ne") {
		lexer.read("ne");
		A();
		BuildTree("ne", 2);
	}
}

void Parser::A() {
	if(lexer.isNextToken("+") == "+") {
		lexer.read("+");
		At();
	} else if(lexer.isNextToken("-") == "-") {
		lexer.read("-");
		At();
		BuildTree("neg", 1);
	} else {
		At();
	}
	
	while(lexer.isNextToken("+") == "+" || lexer.isNextToken("-") == "-") {
		if(lexer.isNextToken("+") == "+") {
			lexer.read("+");
			At();
			BuildTree("+", 2);
		} else {
			lexer.read("-");
			At();
			BuildTree("-", 2);
		}
	}
}

void Parser::At() {
	Af();
	if(lexer.isNextToken("*") == "*") {
		lexer.read("*");
		At();
		BuildTree("*", 2);
	} else if(lexer.isNextToken("/") == "/") {
		lexer.read("/");
		At();
		BuildTree("/", 2);
	}
}

void Parser::Af() {
	Ap();
	if(lexer.isNextToken("**") == "**") {
		lexer.read("**");
		Af();
		BuildTree("**", 2);
	}
}

void Parser::Ap() {
	R();
	while(lexer.isNextToken("@") == "@") {
		lexer.read("@");
		lexer.readIdentifier();
		R();
		BuildTree("@", 2);
	}
}

void Parser::R() {
	Rn();
	while(lexer.isNextToken(IDENTIFIER) != ""
		|| lexer.isNextToken(NUMBER) != ""
		|| lexer.isNextToken(STRING) != "" 
		|| lexer.isNextToken("true") == "true"
		|| lexer.isNextToken("false") == "false"
		|| lexer.isNextToken("nil") == "nil"
		|| lexer.isNextToken("dummy") == "dummy"
		|| lexer.isNextToken("(") == "(") {
		
		Rn();
		BuildTree("gamma", 2);
	}
}

void Parser::Rn() {
	if(lexer.isNextToken(IDENTIFIER) != "") {
		lexer.readIdentifier();
	} else if(lexer.isNextToken(NUMBER) != "") {
		lexer.readNumber();
	} else if(lexer.isNextToken(STRING) != "") {
		lexer.readString();
	} else if(lexer.isNextToken("true") == "true") {
		lexer.read("true");
		BuildTree("true", 0);
	} else if(lexer.isNextToken("false") == "false") {
		lexer.read("false");
		BuildTree("false", 0);
	} else if(lexer.isNextToken("nil") == "nil") {
		lexer.read("nil");
		BuildTree("nil", 0);
	} else if(lexer.isNextToken("dummy") == "dummy") {
		lexer.read("dummy");
		BuildTree("dummy", 0);
	} else if(lexer.isNextToken("(") == "(") {
		lexer.read("(");
		E();
		lexer.read(")");
	} else if(!reachedEof) {
		cout << "Error in Rn.\n";
	} else {
		//reached eof
		//set eof back to false ?
	}
}

void Parser::D() {
	Da();
	if(lexer.isNextToken("within") == "within") {
		lexer.read("within");
		D();
		BuildTree("within", 2);
	}
}

void Parser::Da() {
	Dr();
	if(lexer.isNextToken("and") == "and") {
		int n =1;
		while(lexer.isNextToken("and") == "and") {
			lexer.read("and");
			Dr();
			n++;
		}
		BuildTree("and", n);
	}
}

void Parser::Dr() {
	if(lexer.isNextToken("rec") == "rec") {
		lexer.read("rec");
		Db();
		BuildTree("rec", 1);
	} else {
		Db();
	}
	
}

void Parser::Db() {
	if(lexer.isNextToken("(") == "(") {
		lexer.read("(");
		D();
		lexer.read(")");
	} else {
		//look two tokens ahead. Depending on the token after the first identifier, decide where to go.
		string nextIdentifier = lexer.isNextToken(IDENTIFIER);
		lexer.readIdentifier();
		if(lexer.isNextToken(IDENTIFIER) != "" || lexer.isNextToken("(") == "(") {
			int n = 0;
			while(lexer.isNextToken(IDENTIFIER) != "" || lexer.isNextToken("(") == "(") {
				Vb();
				n++;
			}
			lexer.read("=");
			E();
			BuildTree("function_form", n + 2);
		} else {
			if(lexer.isNextToken(",") == ",") {
				//before calling Vl, put back the first identifier you pulled out earlier.
				lexer.revert(nextIdentifier);
				Vl();
			}
			lexer.read("=");
			E();
			BuildTree("=", 2);
		}
	}
}

void Parser::Vb() {
	if(lexer.isNextToken("(") == "(") {
		lexer.read("(");
		if(lexer.isNextToken(")") == (")")) {
			lexer.read(")");
			//TODO: build tree "()"		
			BuildTree("()", 0);
		} else {
			Vl();
			lexer.read(")");
		}

	} else{
		lexer.readIdentifier();
	}
}

void Parser::Vl() {
	lexer.readIdentifier();
	int n = 1;
	if(lexer.isNextToken(",") == ",") {
		while(lexer.isNextToken(",") == ",") {
			lexer.read(",");
			lexer.readIdentifier();
			n++;
		}
		BuildTree(",", n);
	}
}

//------------------------ Helper functions -------------------//

void Parser::killYourself() {
	in.close();
	exit(0);
}

void Parser::Helper(string file) {
	out.open(file.c_str(), ios::app);
	out << " ";
	out.close();
}

void Parser::BuildTree(string name, int count){
    
    Node* n = new Node;
    n->name = name;
    n->count = count;
    n->child = new Node*[count];
    
    for(int i = count-1; i >= 0; i--){
        Node* temp = s.top();
        s.pop();
        n->child[i] = temp;
    }
    s.push(n);
}

void Parser::DispTree(Node *node){
    DispTree(node, 0);
    cout<<endl;
}
void Parser::DispTree(Node *node, int level){
    if(node == NULL)
        return;
    for(int i = 1; i <= level; i++)
        cout<<".";
    cout<<node->name<<endl;

    if(node->child == NULL)
        return;
    
    for(int i = 0; i < node->count; i++){
        DispTree(node->child[i], level+1);
    }
    
}

int main(int argc, char** argv) {
	Parser parser;
	bool dispTree = false;
	reachedEof = false;
	string file;

	for(int count = 1; count < argc; count++) {
		if(string(argv[count]) == "-ast") {
			dispTree = true;
		} else if(string(argv[count])[0] != '-') {
			file = string(argv[count]);
			parser.Helper(file);
    		in.open(file.c_str());
		}
	}

    parser.E();
    if(dispTree) {
    	parser.DispTree(s.top());
    }
    in.close();
}                                                                                                                                                                   