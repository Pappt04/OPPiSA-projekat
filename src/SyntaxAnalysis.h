#pragma once
#include "LexicalAnalysis.h"

using namespace std;

class SyntaxAnalysis
{
private:
	LexicalAnalysis& lexicalAnalysis;

	bool errorFound;

	TokenList::iterator tokenIterator;

	Token currentToken;
public:

	/**
	* Constructor
	*/
	SyntaxAnalysis(LexicalAnalysis& lex);

	/**
	* Method to start the syntax analysis
	*/
	bool Do();

	/**
	* Write some section starter characters
	*/
	void printMessageHeader();
private:
	/**
	* Token token - the token where the error occured
	* Print syntax ERROR to standard output
	*/
	void printSyntaxError(Token t);

	/**
	* Token token - token that needs to be written
	* Print token info to standard output
	*/
	void printTokenInfo(Token t);

	/**
	* Eats the current token if its type is "t"
	* otherwise reports syntax error
	*
	* param[in] - t - the expected token type
	*/
	void eat(TokenType t);


	/**
	* Returns the next token from the Lexical Analysis list
	*/
	Token getNextToken();

	/**
	 * Nonterminal Q
	 */
	void Q();

	/**
	 * Nonterminal S
	 */
	void S();

	/**
	 * Nonterminal L
	 */
	void L();

	/**
	 * Nonterminal E
	 */
	void E();
};

