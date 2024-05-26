#ifndef __IR__
#define __IR__

#include <iostream>
#include "LexicalAnalysis.h"
#include "Types.h"

using namespace std;

/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

private:
	int m_value;
	VariableType m_type;
	std::string m_name;
	int m_position;
	Regs m_assignment;

public:

	/**
	* Constructors
	*/
	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign), m_value(-69) {}
	Variable(std::string name, int pos) : m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign), m_value(-69) {}
	Variable(std::string name, int pos, VariableType v_type, int val = 0) : m_name(name), m_type(v_type), m_position(pos), m_assignment(no_assign), m_value(val) {}
	/**
	* Getters 
	*/
	std::string getName() const;
	int getValue() const;
	VariableType getType() const;
	int getPosition() const;
	Regs getAssignment() const;

	/**
	* Setters
	*/
	void setName(std::string s);
	void setValue(int i);
	void setType(VariableType vt);
	void setPosition(int p);
	void setAssignment(Regs r);

	/**
	* Operator overload
	*/
	friend ostream& operator<<(ostream& out, const Variable& v);

};


/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;


/**
 * This class represents one instruction in program code.
 */
class Instruction
{
private:
	int m_position;
	InstructionType m_type;

	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;

public:
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	Instruction(int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src) {
		fillVariables_Fill_Def();
	}

	/**
	* Getters
	*/
	InstructionType getType() const;
	int getPosition() const;
	Variables getDst() const;
	Variables getSrc() const;
	Variables getUse() const;
	Variables getDef() const;
	Variables getIn() const;
	Variables getOut() const;
	list<Instruction*> getSucc() const;
	list<Instruction*> getPred() const;
	
	/**
	* Setters
	*/
	void setSucc(Instruction* succ);
	void setPred(Instruction* pred);
	
	void fillVariables_Fill_Def();

	friend ostream& operator<<(ostream& out, const Instruction& ins);
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;

#endif
