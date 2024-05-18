#include "SyntaxAnalysis.h"

#include "LexicalAnalysis.h"
#include "Token.h"

using namespace std;

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex):lexicalAnalysis(lex)
{
	errorSyntax = false;
	tokenIterator = lexicalAnalysis.getTokenList().begin();
}

bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();
	Q();
	return !errorSyntax;
}

void printSyntaxError();

void printTokenInfo();

void eat(TokenType t);

void Q();

void S();

void L();

void E();