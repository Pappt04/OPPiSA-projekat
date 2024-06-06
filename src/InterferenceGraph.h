/*Author: Papp Tamas Index: RA004/2022 Datum: 2024.06.05*/
#pragma once

#include "IR.h"

typedef vector<std::vector<int>> InterferenceMatrix;
typedef stack<Variable*> VariableStack;

class InterferenceGraph
{
private:
	Variables& reg_vars;
	InterferenceMatrix inf_mat;
	VariableStack var_stack;

public:
	
	/**
	* Constructor that initializes the register variables
	*/
	InterferenceGraph(Variables& vars);
	
	/**
	* Function that build interference graph
	* Instrs - instructions for the graph
	*/
	void buildGraph(Instructions& instrs);

	/**
	* Creates stack of register variables
	*/
	void buildVarStack();

	/**
	* Allocates real registers to variables using the interference matrix
	*/
	bool resourceAllocation();

	/**
	* Prints interference graph
	*/
	void printInterferenceMatrix();

private:

	/**
	* Resizes interference matrix to size specified
	*/
	void resizeInterferenceMatrix(int size);

	/**
	* Applies register to variable at the specified position
	*/
	void applyRegtoVariable(int varPos, Regs reg);
};

