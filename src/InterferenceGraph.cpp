#include "InterferenceGraph.h"

using namespace std;

InterferenceGraph::InterferenceGraph(Variables& vars) : reg_vars(vars) {}

void InterferenceGraph::buildGraph(Instructions& instrs)
{
	inf_mat.resize(reg_vars.size());
	for (int i = 0; i < inf_mat.size(); i++)
		inf_mat[i].resize(reg_vars.size());

	for (auto it = instrs.begin(); it != instrs.end(); it++)
	{
		Variables defs = (*it)->getDef();
		for (Variables::iterator dit = defs.begin(); dit != defs.end(); dit++)
		{
			Variables outs = (*it)->getOut();
			for (auto oit = outs.begin(); oit != outs.end(); oit++)
			{
				int defPos = (*dit)->getPosition();
				int outPos = (*oit)->getPosition();

				int ferenc = __EMPTY__;
				if (defPos != outPos)
					ferenc = __INTERFERENCE__;

				inf_mat[defPos][outPos] = ferenc;
				inf_mat[outPos][defPos] = ferenc;
			}
		}
	}
}

void InterferenceGraph::buildVarStack()
{
	for (auto reit = reg_vars.begin();reit!= reg_vars.end();reit++)
	{
		var_stack.push(*reit);
	}
}

bool InterferenceGraph::resourceAllocation()
{
	list<Regs> registers;
	list<Variable*> colored;

	for (int i = 1; i <= __REG_NUMBER__; i++)
		registers.push_back((Regs)i);
	
	while (!var_stack.empty())
	{
		Variable* temp = var_stack.top();
		bool interference = false;
		
		for (auto it = colored.begin(); it != colored.end(); it++)
		{
			if (inf_mat[temp->getPosition()][(*it)->getPosition()] == __INTERFERENCE__)
				interference = true;
		}

		if (!interference)
		{
			if (registers.empty())
				return false;
			temp->setAssignment(registers.front());
		}
		else
		{
			if (registers.size() >= 2 )
			{
				registers.pop_front();
				temp->setAssignment(registers.front());
			}
			else
			{
				return false;
			}
		}
		colored.push_back(temp);
		var_stack.pop();
	}
	return true;
}

void InterferenceGraph::printInterferenceMatrix()
{
	cout << "  ";
	for (int i = 0; i < inf_mat.size(); ++i)
		cout << "r" << i << "  ";
	cout << endl;

	for (int i = 0; i < inf_mat.size(); ++i)
	{
		cout << "r" << i << " ";

		for (int j = 0; j < inf_mat[i].size(); ++j)
			cout << inf_mat[i][j] << "   ";

		cout << endl;
	}
}

void InterferenceGraph::resizeInterferenceMatrix(int size)
{
	inf_mat.resize(size);

	for (unsigned i = 0; i < inf_mat.size(); ++i)
		inf_mat[i].resize(size);
}

void InterferenceGraph::applyRegtoVariable(int varPos, Regs reg)
{
	for (auto it = reg_vars.begin(); it != reg_vars.end(); it++)
	{
		if ((*it)->getPosition() == varPos)
			(*it)->setAssignment(reg);
	}
}
