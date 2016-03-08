//parser.cpp

#include <iostream>
#include <fstream>
#include "lexer.cpp"
#include <vector>

using namespace std;
ifstream in;
Lexer lexer;

void E() {

}

void D() {

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
			lexer.read(")"));
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
	if(lexer.isNextToken(IDENTIFIER) != "") {
		lexer.read(lexer.isNextToken(IDENTIFIER));

		while(lexer.isNextToken(",") == ",") {
			lexer.read(",");
			if(lexer.isNextToken(IDENTIFIER) != "") {
				lexer.read(lexer.isNextToken(IDENTIFIER));
				//it was an identifier, so continue
			} else {
				cout << "Error, IDENTIFIER expected after \",\"\n";
				break;
			}
		}
		//TODO: buildTree ","
	} else {
		cout << "Error, IDENTIFIER expected\n";
	}
}

int main(int argc, char** argv) {
	string file;
	file = argv[1];
    in.open(file.c_str());
    Parser parser;
    // string check = "let";
    // if(lexer.isNextToken(check)){
    // 	cout << "is " << check << "\n";
    // } else {
    // 	cout << "not " << check << "\n";
    // }

    parser.Vb();
    in.close();
}