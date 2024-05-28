#include "LivenessAnalysis.h"

LivenessAnalysis::LivenessAnalysis(Instructions& instrs, list<Labels>& labs, Variables& mmvrs, Variables& rgvrs) : instructions(instrs), label_list(labs), mem_vars(mmvrs), reg_vars(rgvrs) { fillSuccessor(); fillPredecessor(); }

int LivenessAnalysis::findInstructionPosition(Variables vars)
{
	auto vit = vars.begin();
	for (auto lit = label_list.begin(); lit != label_list.end(); lit++)
	{
		if ((*vit)->getName() == (*lit).name)
			return (*lit).position;
	}
	return -1;
}

string LivenessAnalysis::returnAssignedRegister(string r)
{
	for (auto it = reg_vars.begin(); it != reg_vars.end(); it++)
	{
		if (r == (*it)->getName())
		{
			return "t" + std::to_string((*it)->getAssignment()-1);
		}
	}
	return "-ERROR-";
}

void LivenessAnalysis::fillSuccessor()
{
	auto it_end = instructions.end();
	it_end--;
	for (Instructions::iterator it = instructions.begin(); it != it_end; it++)
	{
		if ((*it)->getType() == I_B || (*it)->getType() == I_BLTZ)
		{
			int pos = findInstructionPosition((*it)->getDst());
			for (auto itp = instructions.begin(); itp != instructions.end(); itp++)
			{
				if ((*itp)->getPosition() == pos)
					(*it)->setSucc((*itp));
			}
		}
		else
		{
			auto temp = ++it;
			it--;
			(*it)->setSucc((*temp));
		}
	}
}

void LivenessAnalysis::fillPredecessor()
{
	auto rit_end = instructions.rend();
	rit_end--;
	for (auto rit = instructions.rbegin(); rit != rit_end; rit++)
	{
		auto temp = ++rit;
		rit--;
		(*rit)->setPred((*temp));
	}
}

bool LivenessAnalysis::variableExists(Variable* var, Variables variables)
{
	for (auto it = variables.begin(); it != variables.end(); it++)
	{
		if ((var->getName() == (*it)->getName()) && (var->getType() == (*it)->getType()))
			return true;
	}
	return false;
}

void LivenessAnalysis::Do()
{
	while (true)
	{
		for (auto rit = instructions.rbegin(); rit != instructions.rend(); rit++)
		{
			Variables& oldIN = (*rit)->getOut();
			Variables& oldOUT = (*rit)->getIn();
			
			Variables newIN,newOUT;

			/* out[n] <- U sUsucc[n]in[s]*/
			for (auto sit = (*rit)->getSucc().begin(); sit != (*rit)->getSucc().end(); sit++)
			{
				newOUT.insert(oldOUT.end(), (*sit)->getIn().begin(), (*sit)->getIn().end());
			}

			newOUT.sort();
			newOUT.unique();

			/*in[n] <- use[n] U (out[n]-def[n])*/
			Variables& use = (*rit)->getUse();
			Variables& def = (*rit)->getDef();
			
			newIN = use;
			Variables OUTminDEF;
			/*(out[n]-def[n])*/
			for (auto vit = oldOUT.begin(); vit != oldOUT.end(); vit++)
			{
				if (!variableExists(*vit, def))
				{
					OUTminDEF.push_back(*vit);
					newIN.push_back(*vit);
				}
			}
			newIN.sort();
			newIN.unique();
			
			if ((*rit)->getIn() == newIN && (*rit)->getOut() == newOUT)
				return;

			oldIN = newIN;
			oldOUT = newOUT;
		}
	}
}
