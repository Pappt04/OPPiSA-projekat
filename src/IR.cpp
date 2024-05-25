#include "IR.h"

#include "Types.h"
#include <iostream>

/*
* enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

	int m_value;
	VariableType m_type;
	std::string m_name;
	int m_position;
	Regs m_assignment;
*/

std::string Variable::getName() const { return m_name; }

int Variable::getValue() const { return m_value; }

Variable::VariableType Variable::getType() const { return m_type; }

int Variable::getPosition() const { return m_position; }

Regs Variable::getAssignment() const { return m_assignment; }

void Variable::setName(std::string s) { m_name = s; }

void Variable::setValue(int i) { m_value = i; }

void Variable::setType(VariableType vt) { m_type = vt;
}

void Variable::setPosition(int p) {	m_position = p; }

void Variable::setAssignment(Regs r) { m_assignment = r; }

ostream& operator<<(ostream& out, const Variable& v)
{
	const string type[] = { "MEM_VAR", "REG_VAR", "NO_TYPE" };
	const string assgn[] = { "no_assign", "t0", "t1", "t2", "t3" };
	out << "----------------------------------------------" << endl;
	out << "Type: " << type[v.m_type] << endl;
	out << "Name: " << v.m_name << endl;
	out << "Position: " << v.m_position << endl;
	out << "Assignment: " << assgn[v.m_assignment] << endl;
	out << "Value: " << v.m_value << endl;

	return out;
}







//--------------------------------------------------------------------------
// Class Instruction starts here


/*
* 	int m_position;
	InstructionType m_type;

	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;
*/

InstructionType Instruction::getType() const { return m_type; }

int Instruction::getPosition() const{ return m_position; }

Variables Instruction::getDst() const {	return m_dst; }

Variables Instruction::getSrc() const {	return m_src; }

Variables Instruction::getUse() const { return m_use; }

Variables Instruction::getDef() const {	return m_def; }

Variables Instruction::getIn() const { return m_in; }

Variables Instruction::getOut() const {	return Variables(); }

list<Instruction*> Instruction::getSucc() const { return m_succ; }

list<Instruction*> Instruction::getPred() const { return m_pred; }

void Instruction::setSucc(Instruction* succ) { m_succ.push_back(succ); }

void Instruction::setPred(Instruction* pred) { m_pred.push_back(pred); }

void Instruction::fillVariables_Fill_Def()
{
	switch (m_type)
	{
	case I_ADD: //add rid, rid, rid
	case I_ADDI: // addi rid, rid, num
	case I_SUB: // sub rid, rid, rid
	case I_OR: // or rs, rt
	case I_NOR: // nor rd, rs, rt
		for (Variables::iterator it = m_src.begin(); it != m_src.end(); it++)
		{
			m_use.push_back(*it);
		}
		for (auto it = m_dst.begin(); it != m_dst.end(); it++)
		{
			m_def.push_back(*it);
		}
		break;
	case I_LA: // la rid, mid
	case I_LI: // li rid, num
	case I_LW: // lw rid, num(rid)
	case I_SW: // sw rid, num(rid)
		for (auto it = m_dst.begin(); it != m_dst.end(); it++)
		{
			m_def.push_back(*it);
		}
		break;
	case I_B:
	case I_BLTZ:
		for (Variables::iterator it = m_src.begin(); it != m_src.end(); it++)
		{
			m_use.push_back(*it);
		}
		break;
	default:
		break;
	}
}
