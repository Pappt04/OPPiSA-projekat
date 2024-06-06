/*Author: Papp Tamas Index: RA004/2022 Datum: 2024.06.05*/
#pragma once

#include "LexicalAnalysis.h"
#include "IR.h"
#include "LivenessAnalysis.h"
#include "Labels.h"
using namespace std;

/*typedef struct Label
{
	string name;
	int position;

	Label(string n, int pos) :name(n), position(pos) {}
};

class Label
{
public:
	string name;
	int position;

	Label(string n, int pos) :name(n), position(pos) {}
};
*/
class SyntaxAnalysis
{
private:
	LexicalAnalysis& lexicalAnalysis;

	bool errorFound;

	TokenList::iterator tokenIterator;

	Token currentToken;

	Instructions& instructions;

	Variables mem_vars;
	Variables reg_vars;
	
	list<string> func_list;
	list<Labels> label_list;

	int registerCounter;

	int instructionPosition;
public:

	/**
	* Constructor
	*/
	SyntaxAnalysis(LexicalAnalysis& lex, Instructions& instrs);

	/**
	* Method to start the syntax analysis
	*/
	bool Do();

	/**
	* Write some section starter characters
	*/
	void printMessageHeader();

	/**
	* Returns a list of register variables
	*/
	Variables& getRegVariables();

	/**
	* Function that creates MIPS file
	* string path - output path for the file
	*/
	void createMipsFile(const string path);
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
	* Lets you peek the next token without consuming it
	*/
	Token peek();

	/**
	* Lets you look back at the previous token
	*/
	Token glanceBack();

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

	/**
	* Method to fill up a list where the memory elements are held
	* Token t1 - memoryID
	* Token t2 - value
	*/
	void fillMemoryVarList(Token& t1, Token& t2);

	/**
	* Method to fill up a list where the registers are held
	* Token r - registerID
	*/
	void fillRegisterVarList(Token& r);

	/**
	* Method to fill up a list where all the function names are held
	* string name - name of function
	* int pos - position in program
	*/
	void fillFunctionList(string name, int pos);

	/**
	* Method to fill up a list where all the Label names are held
	* string name - name of label
	* int pos - position in program
	*/
	void fillLabelList(string name, int pos);

	/**
	* Get register position
	* const string varName - name of register of position needed
	*/
	int getRegisterPosition(const string& varName);

	/**
	* Method that creates instructions based on list of tokens scanned in
	* type - Type of instruction expected or should be created
	* dst - destination registers in the program
	* src - source of data to instruction
	*/
	void instructionFactory(InstructionType type, vector<Token>& dst, vector<Token>& src);

	/**
	* Returns the name of assigned register variable
	*/
	string returnAssignedRegister(string r);

	/**
	* Checks if label belongs in the file
	* 
	*/
	string checkLabels(int pos, list<Labels> labList);
};

