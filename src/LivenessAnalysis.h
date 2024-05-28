#pragma once

#include "IR.h"
#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include <list>
#include "Labels.h"
using namespace std;

class LivenessAnalysis
{
private:
	Instructions instructions;
	list<Labels> label_list;
	Variables mem_vars;
	Variables reg_vars;
public:

	LivenessAnalysis(Instructions& instrs, list<Labels>& labs, Variables& mmvrs, Variables& rgvrs);

	/**
	* Does Liveness analysis
	*/
	void Do();
	
private:
	/**
	* Returns the position of instruction in program
	*/
	int findInstructionPosition(Variables vars);

	/**
	* Returns the name of assigned register variable
	*/
	string returnAssignedRegister(string r);

	/**
	* Fills the list of successor variables
	*/
	void fillSuccessor();

	/**
	* Fills the list of predecessor variables
	*/
	void fillPredecessor();

	/**
	* Checks if Variable exists in variable list
	*/
	bool variableExists(Variable* var, Variables variables);
};