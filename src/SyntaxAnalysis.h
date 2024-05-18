#pragma once

#include "LexicalAnalysis.h"
#include "Token.h"

using namespace std;

class SyntaxAnalysis
{
private:
	LexicalAnalysis& lexicalAnalysis;
	bool errorSyntax;
	TokenList::iterator tokenIterator;
	Token currentToken;
public:
	SyntaxAnalysis(LexicalAnalysis& lex);

	bool Do();

private:
	void printSyntaxError();

	void printTokenInfo();

	Token getNextToken();
	
	void eat(TokenType t);

	void Q();

	void S();

	void L();

	void E();
};

