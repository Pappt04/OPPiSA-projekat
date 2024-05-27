#include "LivenessAnalysis.h"

LivenessAnalysis::LivenessAnalysis(Instructions& instrs, list<Labels> labs): instructions(instrs), label_list(labs) {}

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
	//for(auto it= )
	return "";
}

void LivenessAnalysis::fillSuccessor()
{
}

void LivenessAnalysis::fillPredecessor()
{
}
