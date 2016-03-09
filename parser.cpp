//parser.cpp

#include <iostream>
#include <fstream>
#include "lexer.cpp"
#include <vector>
#include <algorithm>

using namespace std;
ifstream in;
Lexer lexer;

void Parser::E() {

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

	} else if(lexer.isNextToken("-") == "-") {
		lexer.read("-");
	}
	At();
	//TODO not complete
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
	lexer.read("**");
	Af();
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
	while(in.peek() == ' ') {
		in.get();
		Rn();
	}
}

void Parser::Rn() {
	if(lexer.isNextToken(IDENTIFIER) != "") {
		lexer.read(lexer.isNextToken(IDENTIFIER));
	} else if(lexer.isNextToken(NUMBER) != "") {
		lexer.read(lexer.isNextToken(NUMBER));
	} else if(lexer.isNextToken(STRING) != "") {
		lexer.read(lexer.isNextToken(STRING));
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
		if(lexer.isNextToken(")") == ")") {
			lexer.read(")");
		} else {
			cout << "Error, expecting ')'\n";
		}
	} else {
		cout << "Error.";
	}
}

void Parser::D() {
	Da();
	lexer.read("within");
	D();
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
	if(lexer.isNextToken(IDENTIFIER) != "") {
		//found an identifier
		lexer.read(lexer.isNextToken(IDENTIFIER));
		Vb();

		while(lexer.isNextToken("=") != "=" || in.peek() == EOF) {
			Vb();
		}
		lexer.read("=");
		E();
	} else if (lexer.isNextToken("(") == "(") {
		lexer.read(lexer.isNextToken("("));
		D();
		lexer.read(")");
	} else {
		Vl();
		lexer.read("=");
		E();
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
	while(lexer.isNextToken(",") == ",") {
		lexer.read(",");
		lexer.readIdentifier();
	}
}

void Parser::Test() {
	cout << string(1,in.peek()) << "\n";
	in.get(); //a
	cout << string(1,in.peek()) << "\n";
	in.get(); //l

	if(in.peek() != EOF) {
		cout << "inside\n";
		in.get();
	}
	// cout << string(1,in.peek()) << "\n"; //peeked eof
	in.putback('l');
	// cout << string(1,in.peek()) << "\n";
	if(in.fail()) {
		cout << "failed\n";
	}
}

int main(int argc, char** argv) {
	string file;
	file = argv[1];
    in.open(file.c_str());
    Parser parser;

    parser.Vl();
    in.close();
}