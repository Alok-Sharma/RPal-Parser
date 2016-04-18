//parser.h
#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
using namespace std;

void Test();
void BuildTree(string name, int popCount);
void DispTree(Node *node, int level);

	//Parser functions
void E();
void Ew();

void T();
void Ta();
void Tc();

void B();
void Bt();
void Bs();
void Bp();

void A();
void At();
void Af();
void Ap();

void R();
void Rn();

void D();
void Da();
void Dr();
void Db();

void Vb();
void Vl();

#endif