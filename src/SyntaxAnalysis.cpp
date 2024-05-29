#include "SyntaxAnalysis.h"

#include <iomanip>
#include <iostream>

#include "LexicalAnalysis.h"
#include "IR.h"
#include "LivenessAnalysis.h"

using namespace std;

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex, Instructions& instrs) : lexicalAnalysis(lex), instructions(instrs)
{
	errorFound = false;
	tokenIterator = lexicalAnalysis.getTokenList().begin();
	registerCounter = 0;
	instructionPosition = 0;
}

bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();
	Q();
	if (!errorFound)
	{
		cout << "Syntax analysis finished";
		cout << " successfully";
		cout << "!" << endl;
	}
	else
	{
		cout << "Syntax analysis finished with";
		cout << " errors";
		cout << "!" << endl;
	}

	cout << endl << "Liveness Analysis started..." << endl;
	
	LivenessAnalysis liv(instructions, label_list, mem_vars, reg_vars);
	
	liv.Do();
	
	cout << endl << "Liveness Analysis finished successfully!" << endl;

	return !errorFound;
}

void SyntaxAnalysis::printSyntaxError(Token t)
{
	cout << "Syntax error! Unexptected Token:";
	t.printTokenInfo();
	cout << endl;
}

void SyntaxAnalysis::printTokenInfo(Token t)
{
	//cout << setw(20) << left << token.tokenTypeToString();
	cout << setw(25) << right << t.getValue() << endl;
}

Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}

void SyntaxAnalysis::eat(TokenType tt)
{
	if (errorFound)
		return;

	if (currentToken.getType() == tt)
	{
		//cout << currentToken.getValue() << endl;
		if (tt != T_END_OF_FILE)
			currentToken = getNextToken();
	}
	else
	{
		printMessageHeader();
		printSyntaxError(currentToken);
		errorFound = true;
	}
}

Token SyntaxAnalysis::peek()
{
	TokenList::iterator it = tokenIterator;
	it++;
	return *it;
}

Token SyntaxAnalysis::glanceBack()
{
	auto it = tokenIterator;
	it--;
	return *it;
}

void SyntaxAnalysis::Q()
{
	if (!errorFound)
	{
		S();
		eat(T_SEMI_COL);
		L();
	}
}

void SyntaxAnalysis::S()
{
	if (errorFound)
		return;

	Token t;
	switch (currentToken.getType())
	{
	case T_MEM:
		eat(T_MEM);
		t = currentToken;
		eat(T_M_ID);
//		if (peek().getType() == T_NUM)
			fillMemoryVarList(t, currentToken);
		eat(T_NUM);
		break;
	case T_REG:
		eat(T_REG);
//		if (peek().getType() == T_R_ID)
			fillRegisterVarList(currentToken);
		eat(T_R_ID);
		break;
	case T_FUNC:
		eat(T_FUNC);
//		if (peek().getType() == T_ID)
			fillFunctionList(currentToken.getValue(), instructionPosition);
		eat(T_ID);
		break;
	case T_ID:
		fillLabelList(currentToken.getValue(), instructionPosition);
		eat(T_ID);
		eat(T_COL);
		E();
		break;
	default:
		E();
		break;
	}
}

void SyntaxAnalysis::L()
{
	if (errorFound)
		return;

	switch (currentToken.getType())
	{
	case T_END_OF_FILE:
		eat(T_END_OF_FILE);
		break;
	default:
		Q();
		break;
	}
}

void SyntaxAnalysis::E()
{
	if (errorFound)
		return;

	vector<Token> src;
	vector<Token> dst;
	switch (currentToken.getType())
	{
	case T_ADD:
		eat(T_ADD);

//		if (peek().getType() == T_R_ID)
			dst.push_back(currentToken);

		eat(T_R_ID);
		eat(T_COMMA);

//		if (peek().getType() == T_R_ID)
			src.push_back(currentToken);

		eat(T_R_ID);
		eat(T_COMMA);

//		if (peek().getType() == T_R_ID)
			src.push_back(currentToken);

		eat(T_R_ID);
		instructionFactory(I_ADD, dst, src);
		break;
	case T_ADDI:
		eat(T_ADDI);

//		if (peek().getType() == T_R_ID)
			dst.push_back(currentToken);

		eat(T_R_ID);
		eat(T_COMMA);

//		if (peek().getType() == T_R_ID)
			src.push_back(currentToken);

		eat(T_R_ID);
		eat(T_COMMA);

//		if (peek().getType() == T_NUM)
			src.push_back(currentToken);

		eat(T_NUM);
		instructionFactory(I_ADDI, dst, src);
		break;
	case T_SUB:
		eat(T_SUB);

//		if (peek().getType() == T_R_ID)
			dst.push_back(currentToken);

		eat(T_R_ID);
		eat(T_COMMA);

//		if (peek().getType() == T_R_ID)
			src.push_back(currentToken);

		eat(T_R_ID);
		eat(T_COMMA);

//		if (peek().getType() == T_R_ID)
			src.push_back(currentToken);

		eat(T_R_ID);
		instructionFactory(I_SUB, dst, src);
		break;
	case T_LA:
		eat(T_LA);
//		if (peek().getType() == T_R_ID)
			dst.push_back(currentToken);

		eat(T_R_ID);
		eat(T_COMMA);

//		if (peek().getType() == T_M_ID)
			src.push_back(currentToken);

		eat(T_M_ID);
		instructionFactory(I_LA, dst, src);
		break;
	case T_LW:
		eat(T_LW);

//		if (peek().getType() == T_R_ID)
			dst.push_back(currentToken);

		eat(T_R_ID);
		eat(T_COMMA);
//		if (peek().getType() == T_NUM)
			src.push_back(currentToken);

		eat(T_NUM);
		eat(T_L_PARENT);

//		if (peek().getType() == T_R_ID)
			src.push_back(currentToken);

		eat(T_R_ID);
		eat(T_R_PARENT);
		instructionFactory(I_LW, dst, src);
		break;
	case T_LI:
		eat(T_LI);
		
//		if (peek().getType() == T_R_ID)
			dst.push_back(currentToken);
		
		eat(T_R_ID);
		eat(T_COMMA);

//		if (peek().getType() == T_NUM)
			src.push_back(currentToken);
		
		eat(T_NUM);
		instructionFactory(I_LI, dst, src);
		break;
	case T_SW:
		eat(T_SW);

//		if (peek().getType() == T_R_ID)
			dst.push_back(currentToken);
		
		eat(T_R_ID);
		eat(T_COMMA);
		
//		if (peek().getType() == T_NUM)
			src.push_back(currentToken);
		
		eat(T_NUM);
		eat(T_L_PARENT);
		
//		if (peek().getType() == T_R_ID)
			src.push_back(currentToken);
		
		eat(T_R_ID);
		eat(T_R_PARENT);
		instructionFactory(I_SW, dst, src);
		break;
	case T_B:
		eat(T_B);
		
//		if (peek().getType() == T_ID)
			dst.push_back(currentToken);
		
		eat(T_ID);
		instructionFactory(I_B, dst, src);
	case T_BLTZ:
		eat(T_BLTZ);
		
//		if (peek().getType() == T_R_ID)
			src.push_back(currentToken);
		
		eat(T_R_ID);
		eat(T_COMMA);
		
//		if (peek().getType() == T_ID)
			dst.push_back(currentToken);
		
		eat(T_ID);
		instructionFactory(I_BLTZ, dst, src);
		break;
	case T_NOP:
		eat(T_NOP);
		instructionFactory(I_NOP, dst, src);
		break;
	//TODO NEEDED THE 3 EXTRA INSTRUCTIONS HERE
	default:
		errorFound = true;
		break;
	}
}

void SyntaxAnalysis::fillMemoryVarList(Token& t1, Token& t2)
{
	Variable* var = new Variable(t1.getValue(), 0, Variable::MEM_VAR, stoi(t2.getValue()));
	mem_vars.push_back(var);
}

void SyntaxAnalysis::fillRegisterVarList(Token& r)
{
	Variable* var = new Variable(r.getValue(), registerCounter, Variable::REG_VAR);
	reg_vars.push_back(var);
	registerCounter++;
}

void SyntaxAnalysis::fillFunctionList(string name, int pos)
{
	func_list.push_back(name);
	label_list.push_back(Labels(name, pos));
}

void SyntaxAnalysis::fillLabelList(string name, int pos)
{
	label_list.push_back(Labels(name, pos));
}

int SyntaxAnalysis::getRegisterPosition(const string& varName)
{
	for (Variables::iterator it = reg_vars.begin(); it != reg_vars.end(); it++)
	{
		if ((*it)->getName() == varName)
			return (*it)->getPosition();
	}
	throw exception("THIS IS NOT SUPPOSED TO HAPPEN (i think, idk :/ )");
}

void SyntaxAnalysis::instructionFactory(InstructionType Itype, vector<Token>& dst, vector<Token>& src)
{
	Instruction* instr;
	Variables* dstVars = new Variables;
	Variables* srcVars = new Variables;
	Variable* var;

	if (Itype == I_NOP)
	{
		instr = new Instruction(instructionPosition, Itype, *dstVars, *srcVars);
		instructions.push_back(instr);
		instructionPosition++;
		return;
	}

	for (auto it = dst.begin(); it != dst.end(); it++)
	{
		if((*it).getType() == T_R_ID )
		{
			var = new Variable((*it).getValue(), getRegisterPosition((*it).getValue()), Variable::REG_VAR);
		}
		else
		{
			var = new Variable((*it).getValue(), 0, Variable::NO_TYPE);
		}
		dstVars->push_back(var);
	}
	for (auto it = src.begin(); it != src.end(); it++)
	{
		if ((*it).getType() == T_R_ID)
		{
			var = new Variable((*it).getValue(), getRegisterPosition((*it).getValue()), Variable::REG_VAR);
		}
		else
		{
			var = new Variable((*it).getValue(), 0, Variable::NO_TYPE);
		}
		srcVars->push_back(var);
	}
	/*switch (Itype)
	{
	case I_ADD:
		var = new Variable((*itd).getValue(), getRegisterPosition((*itd).getValue()), Variable::REG_VAR);
		dstVars->push_back(var);
		var = new Variable((*its).getValue(), getRegisterPosition((*its).getValue()), Variable::REG_VAR);
		srcVars->push_back(var);
		its++;
		var = new Variable((*its).getValue(), getRegisterPosition((*its).getValue()), Variable::REG_VAR);
		srcVars->push_back(var);
		break;
	case I_ADDI:
		var = new Variable((*itd).getValue(), getRegisterPosition((*itd).getValue()), Variable::REG_VAR);
		dstVars->push_back(var);
		var = new Variable((*its).getValue(), getRegisterPosition((*its).getValue()), Variable::REG_VAR);
		srcVars->push_back(var);
		its++;
		var = new Variable((*its).getValue(),0, Variable::NO_TYPE);
		srcVars->push_back(var);
		break;
	case I_SUB:
		var = new Variable((*itd).getValue(), getRegisterPosition((*itd).getValue()), Variable::REG_VAR);
		dstVars->push_back(var);
		var = new Variable((*its).getValue(), getRegisterPosition((*its).getValue()), Variable::REG_VAR);
		srcVars->push_back(var);
		its++;
		var = new Variable((*its).getValue(), getRegisterPosition((*its).getValue()), Variable::REG_VAR);
		srcVars->push_back(var);
		break;
	case I_LA:
		var = new Variable((*itd).getValue(), getRegisterPosition((*itd).getValue()), Variable::REG_VAR);
		dstVars->push_back(var);
		var = new Variable((*its).getValue(), 0, Variable::NO_TYPE);
		srcVars->push_back(var);
		break;
	case I_LW:
		var = new Variable((*itd).getValue(), getRegisterPosition((*itd).getValue()), Variable::REG_VAR);
		dstVars->push_back(var);
		var = new Variable((*its).getValue(),0, Variable::NO_TYPE);
		srcVars->push_back(var);
		its++;
		var = new Variable((*its).getValue(), getRegisterPosition((*its).getValue()), Variable::REG_VAR);
		srcVars->push_back(var);
		break;
	case I_LI:
		var = new Variable((*itd).getValue(), getRegisterPosition((*itd).getValue()), Variable::REG_VAR);
		dstVars->push_back(var);
		var = new Variable((*its).getValue(), getRegisterPosition(((*its).getValue())), Variable::REG_VAR);
		srcVars->push_back(var);
		break;
	case I_SW:
		var = new Variable((*its).getValue(),)
		break;
	default:
		break;
	}
	*/
	instr = new Instruction(instructionPosition, Itype, *dstVars, *srcVars);
	instructions.push_back(instr);
	instructionPosition++;
}

string SyntaxAnalysis::returnAssignedRegister(string r)
{
	for (auto it = reg_vars.begin(); it != reg_vars.end(); it++)
	{
		if (r == (*it)->getName())
		{
			return "t" + std::to_string((*it)->getAssignment() - 1);
		}
	}
	return "-ERROR-";
}

string SyntaxAnalysis::checkLabels(int pos, list<Labels> labList)
{
	string ret = "";
	for (list<Labels>::iterator it_label = label_list.begin(); it_label != label_list.end(); it_label++)
	{
		if (pos == (*it_label).position)
		{
			ret += (*it_label).name + ":\n";
		}
	}
	return ret;
}

Variables& SyntaxAnalysis::getRegVariables()
{
	return reg_vars;
}

void SyntaxAnalysis::createMipsFile(const string path)
{
	ofstream fout(path);
	if (fout.is_open())
	{
		cout << "Opened file to write MIPS instructions in"<<endl;
		
		fout << ".globl ";
		for (string& fit : func_list)
			fout << fit << endl;

		fout << "\n.data\n";

		for (auto vit = mem_vars.begin(); vit != mem_vars.end(); vit++)
			fout << (*vit)->getName() << ":\t .word " << (*vit)->getValue() << endl;

		fout << "\n.text\n";

		string instrs[] = {"add","addi","sub","la","li","lw","sw","bltz","b","NOP"};
		for (auto it = instructions.begin(); it != instructions.end(); it++)
		{
			fout << checkLabels((*it)->getPosition(), label_list);
			
			fout << "\t" << instrs[(*it)->getType() - I_ADD] << "\t ";
			switch ((*it)->getType())
			{
			case I_ADD:
			case I_SUB:
				fout << "$" << returnAssignedRegister((*it)->getDst().front()->getName()) << ", $" << returnAssignedRegister((*it)->getSrc().front()->getName()) << ", $" << returnAssignedRegister((*it)->getDst().back()->getName()) << endl;
				break;
			case I_ADDI:
				fout << "$" << returnAssignedRegister((*it)->getDst().front()->getName()) << ", $" << returnAssignedRegister((*it)->getSrc().front()->getName()) << ", $" << returnAssignedRegister((*it)->getSrc().back()->getName()) << endl;
				break;
			case I_LW:
				fout << "$" << returnAssignedRegister((*it)->getDst().front()->getName()) << ", " << (*it)->getSrc().front()->getName() << "($" << returnAssignedRegister((*it)->getSrc().back()->getName()) <<")"<< endl;
				break;
			case I_LA:
			case I_LI:
				fout << "$" << returnAssignedRegister((*it)->getDst().front()->getName()) << ", " << (*it)->getSrc().front()->getName() << endl;
				break;
			case I_SW:
				fout << "$" << returnAssignedRegister((*it)->getSrc().front()->getName()) << ", $" << (*it)->getDst().front()->getName() << ", $" << returnAssignedRegister((*it)->getDst().back()->getName()) << endl;
				break;
			case I_B:
				fout << "$" << (*it)->getDst().front()->getName() << endl;
				break;
			case I_BLTZ:
				fout << "$" << returnAssignedRegister((*it)->getSrc().front()->getName()) << ", " << (*it)->getDst().front()->getName() << endl;
				break;
			default:
				break;
			}
		}

	}
	else
	{
		throw exception("File could not be opened");
	}
	fout.close();
}

void SyntaxAnalysis::printMessageHeader()
{
	//cout << "Syntax analysis started\n";
	//cout << setw(LEFT_ALIGN) << left << "Type:";
	//cout << setw(RIGHT_ALIGN) << right << "Value:" << endl;
	cout << setfill('-') << setw(LEFT_ALIGN + RIGHT_ALIGN + 1) << " " << endl;
	cout << setfill(' ');
}