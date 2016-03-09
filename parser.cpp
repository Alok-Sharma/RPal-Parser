//parser.cpp

#include <iostream>
#include <fstream>
#include "lexer.cpp"
#include <vector>
#include <algorithm>

using namespace std;
ifstream in;
ofstream out;
Lexer lexer;
bool reachedEof;

void Parser::E() {
	if(lexer.isNextToken("let") == "let") {
		lexer.read("let");
		D();
		lexer.read("in");
		E();
		//buildtree
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
	} else {
		Ew();
	}
}

void Parser::Ew() {
	T();
	if(lexer.isNextToken("where") == "where") {
		lexer.read("where");
		Dr();
	}
}

void Parser::T() {
	Ta();
	while(lexer.isNextToken(",") == ",") {
		lexer.read(",");
		Ta();
	}
}

void Parser::Ta() {
	Tc();
	while(lexer.isNextToken("aug") == "aug") {
		lexer.read("aug");
		Tc();
	}
}

void Parser::Tc() {
	B();
	if(lexer.isNextToken("->") == "->") {
		lexer.read("->");
		Tc();
		lexer.read("|");
		Tc();
	}
}

void Parser::B() {
	Bt();
	while(lexer.isNextToken("or") == "or") {
		lexer.read("or");
		Bt();
	}
}

void Parser::Bt() {
	Bs();
	while(lexer.isNextToken("&") == "&") {
		lexer.read("&");
		Bs();
	}
}

void Parser::Bs() {
	if(lexer.isNextToken("not") == "not") {
		lexer.read("not");
	}
	Bp();
}

void Parser::Bp() {
	A();

	if(lexer.isNextToken("gr") == "gr") {
		lexer.read("gr");
		A();
	} else if(lexer.isNextToken(">") == ">") {
		lexer.read(">");
		A();
	} else if(lexer.isNextToken("ge") == "ge") {
		lexer.read("ge");
		A();
	} else if(lexer.isNextToken(">=") == ">=") {
		lexer.read(">=");
		A();
	} else if(lexer.isNextToken("ls") == "ls") {
		lexer.read("ls");
		A();
	} else if(lexer.isNextToken("<") == "<") {
		lexer.read("<");
		A();
	} else if(lexer.isNextToken("le") == "le") {
		lexer.read("le");
		A();
	} else if(lexer.isNextToken("<=") == "<=") {
		lexer.read("<=");
		A();
	} else if(lexer.isNextToken("eq") == "eq") {
		lexer.read("eq");
		A();
	} else if(lexer.isNextToken("ne") == "ne") {
		lexer.read("ne");
		A();
	}
}

void Parser::A() {
	if(lexer.isNextToken("+") == "+") {
		lexer.read("+");
		At();
	} else if(lexer.isNextToken("-") == "-") {
		lexer.read("-");
		At();
	} else {
		At();
	}
	
	while(lexer.isNextToken("+") == "+" || lexer.isNextToken("-") == "-") {
		if(lexer.isNextToken("+") == "+") {
			lexer.read("+");
			At();
			//buildtree +
		} else {
			lexer.read("-");
			At();
			//buildtree -
		}
	}
}

void Parser::At() {
	Af();
	if(lexer.isNextToken("*") == "*") {
		lexer.read("*");
		At();
	} else if(lexer.isNextToken("/") == "/") {
		lexer.read("*");
		At();
	}
}

void Parser::Af() {
	Ap();
	if(lexer.isNextToken("**") == "**") {
		lexer.read("**");
		Af();
	}
}

void Parser::Ap() {
	R();
	while(lexer.isNextToken("@") == "@") {
		lexer.read("@");
		lexer.readIdentifier();
		R();
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
	} else if(lexer.isNextToken("false") == "false") {
		lexer.read("false");
	} else if(lexer.isNextToken("nil") == "nil") {
		lexer.read("nil");
	} else if(lexer.isNextToken("dummy") == "dummy") {
		lexer.read("dummy");
	} else if(lexer.isNextToken("(") == "(") {
		lexer.read("(");
		E();
		lexer.read(")");
	} else if(!reachedEof) {
		cout << "Error in Rn\n";
	} else {

	}
}

void Parser::D() {
	Da();
	if(lexer.isNextToken("within") == "within") {
		lexer.read("within");
		D();
	}
}

void Parser::Da() {
	Dr();
	while(lexer.isNextToken("and") == "and") {
		lexer.read("and");
		Dr();
	}
}

void Parser::Dr() {
	if(lexer.isNextToken("rec") == "rec") {
		lexer.read("rec");
	}
	Db();
}

void Parser::Db() {
	if(lexer.isNextToken("(") == "(") {
		lexer.read("(");
		D();
		lexer.read(")");
	} else {
		lexer.readIdentifier();
		if(lexer.isNextToken(IDENTIFIER) != "" || lexer.isNextToken("(") == "(") {
			while(lexer.isNextToken(IDENTIFIER) != "" || lexer.isNextToken("(") == "(") {
				Vb();
			}
			lexer.read("=");
			E();
		} else {
			if(lexer.isNextToken(",") == ",") {
				Vl();
			}
			lexer.read("=");
			E();
		}
	}
}

void Parser::Vb() {
	if(lexer.isNextToken("(") == "(") {
		lexer.read("(");
		if(lexer.isNextToken(")") == (")")) {
			lexer.read(")");
			//TODO: build tree "()"			
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
	if(lexer.isNextToken(",") == ",") {
		while(lexer.isNextToken(",") == ",") {
			lexer.read(",");
			lexer.readIdentifier();
		}
	} else {
		cout << "Error, expecting ','\n";
		killYourself();
	}
}

void Parser::killYourself() {
	in.close();
	exit(0);
}

void Parser::Helper(string file) {
	out.open(file, ios::app);
	out << " ";
	out.close();
}

int main(int argc, char** argv) {
	Parser parser;
	reachedEof = false;

	string file;
	file = argv[1];
	parser.Helper(file);

    in.open(file.c_str());

    parser.E();
    // lexer.isNumber();
    in.close();
}