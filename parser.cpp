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

void Parser::Ap() {
	R();
	while(lexer.isNextToken("@") == "@") {
		lexer.read("@");

	}
}

void Parser::R() {
	Rn();

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
	if(lexer.isNextToken("within") == "within") {
		lexer.read("within");
		D();
	} else if(lexer.isNextToken(";") == ";") {
		lexer.read(";");
	} else {
		cout << "Error, expecting another definition, or a ';'\n";
	}
}

void Parser::Da() {
	Dr();
	while(lexer.isNextToken("and") == "and") {
		lexer.read("and");
		Dr();
	}
	if(lexer.isNextToken(";") == ";") {
		lexer.read(";");
	} else {
		cout << "Error, expecting ';'\n";
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

		if(lexer.isNextToken("=") == "=") {
			lexer.read("=");
			E();
		} else {
			cout << "Error, expecting '='\n";
		}

	} else if (lexer.isNextToken("(") == "(") {
		lexer.read(lexer.isNextToken("("));
		D();

		if(lexer.isNextToken(")") == ")") {
			lexer.read(")");
			if(lexer.isNextToken(";") == ";") {
				lexer.read(";");
			}
		} else {
			cout << "Error, expecting ')'\n";
		}

	} else {
		Vl();

		if(lexer.isNextToken("=") == "=") {
			lexer.read("=");
			E();
		} else {
			cout << "Error, expecting '='\n";
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

			if(lexer.isNextToken(")") == (")")) {
				lexer.read(")");
			} else {
				cout << "Error, expecing )\n";
			}
		}

	} else if(lexer.isNextToken(IDENTIFIER) != "") {
		//got identifier, read
		lexer.read(lexer.isNextToken(IDENTIFIER));
	} else {
		cout << "Error.\n";
	}
}

void Parser::Vl() {
	// if(lexer.isNextToken(IDENTIFIER) != "") {
	// 	lexer.read(lexer.isNextToken(IDENTIFIER));

	// 	while(lexer.isNextToken(",") == ",") {
	// 		lexer.read(",");
	// 		if(lexer.isNextToken(IDENTIFIER) != "") {
	// 			lexer.read(lexer.isNextToken(IDENTIFIER));
	// 			//it was an identifier, so continue
	// 		} else {
	// 			cout << "Error, IDENTIFIER expected after \",\"\n";
	// 			break;
	// 		}
	// 	}
	// 	//TODO: buildTree ","
	// } else {
	// 	cout << "Error, IDENTIFIER expected\n";
	// }

	lexer.readIdentifier();
	while(lexer.isNextToken(",") == ",") {
		lexer.read(",");
		lexer.readIdentifier();
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