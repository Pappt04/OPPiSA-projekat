#include "SyntaxAnalysis.h"

#include <iostream>
#include <iomanip>\

#include "LexicalAnalysis.h"

using namespace std;

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex): lexicalAnalysis(lex)
{
	errorFound = false;
	tokenIterator = lexicalAnalysis.getTokenList().begin();
}

bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();
	Q();
	return !errorFound;
}

void SyntaxAnalysis::printSyntaxError(Token t)
{
	cout << "Syntax error! Unexptected Token:";
	t.printTokenInfo();
	cout << endl;
}

void SyntaxAnalysis::printTokenInfo(Token t)
{
	//cout << setw(20) << left << token.tokenTypeToString();
	cout << setw(25) << right << t.getValue() << endl;
}

Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}

void SyntaxAnalysis::eat(TokenType tt)
{
	if (errorFound)
		return;

	if (currentToken.getType() == tt)
	{
		//cout << currentToken.getValue() << endl;
		if (tt != T_END_OF_FILE)
			currentToken = getNextToken();
	}
	else
	{
		printMessageHeader();
		printSyntaxError(currentToken);
		errorFound = true;
	}
}

void SyntaxAnalysis::Q()
{
	if (!errorFound)
	{
		S();
		eat(T_SEMI_COL);
		L();
	}
}

void SyntaxAnalysis::S()
{
	if (errorFound)
		return;

	switch (currentToken.getType())
	{
	case T_MEM:
		eat(T_MEM);
		eat(T_M_ID);
		eat(T_NUM);
		break;
	case T_REG:
		eat(T_REG);
		eat(T_R_ID);
		break;
	case T_FUNC:
		eat(T_FUNC);
		eat(T_ID);
		break;
	case T_ID:
		eat(T_ID);
		eat(T_COL);
		E();
		break;
	default:
		E();
		break;
	}
}

void SyntaxAnalysis::L()
{
	if (errorFound)
		return;

	switch (currentToken.getType())
	{
	case T_END_OF_FILE:
		eat(T_END_OF_FILE);
		break;
	default:
		Q();
		break;
	}
}

void SyntaxAnalysis::E()
{
	if (errorFound)
		return;

	switch (currentToken.getType())
	{
	case T_ADD:
		eat(T_ADD);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		break;
	case T_ADDI:
		eat(T_ADDI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		break;
	case T_SUB:
		eat(T_SUB);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		break;
	case T_LA:
		eat(T_LA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_M_ID);
		break;
	case T_LW:
		eat(T_LW);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		eat(T_R_ID);
		eat(T_R_PARENT);
		break;
	case T_LI:
		eat(T_LI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		break;
	case T_SW:
		eat(T_SW);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		eat(T_R_ID);
		eat(T_R_PARENT);
		break;
	case T_B:
		eat(T_B);
		eat(T_ID);
	case T_BLTZ:
		eat(T_BLTZ);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_ID);
		break;
	case T_NOP:
		eat(T_NOP);
	default:
		break;
	}
}

void SyntaxAnalysis::printMessageHeader()
{
	//cout << "Syntax analysis started\n";
	//cout << setw(LEFT_ALIGN) << left << "Type:";
	//cout << setw(RIGHT_ALIGN) << right << "Value:" << endl;
	cout << setfill('-') << setw(LEFT_ALIGN + RIGHT_ALIGN + 1) << " " << endl;
	cout << setfill(' ');
}