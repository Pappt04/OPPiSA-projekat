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
public:

	LivenessAnalysis(Instructions& instrs, list<Labels> labs);

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
};

