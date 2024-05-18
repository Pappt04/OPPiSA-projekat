#include <iostream>
#include <exception>

#include "LexicalAnalysis.h"

using namespace std;

void printOutInputFileSourceCode(string fin)
{
	std::ifstream fileReader(fin);

	if (fileReader.is_open())
	{
		string line;
		while (getline(fileReader, line))
		{
			cout << line << endl;
		}
		fileReader.close();
	}
	cout << endl << "The real program start here:" << endl << endl << endl << endl;
}

int main(int arc, char* argv[])
{
	try
	{
		std::string iFile = argv[1];
		std::string oFile = argv[2];
		
		//std::string fileName = ".\\..\\examples\\testing.mavn";
		std::string fileName = ".\\..\\examples\\" + iFile;
		std::string outFileName = ".\\..\\examples\\" + oFile;

		bool retVal = false;

		LexicalAnalysis lex;

		printOutInputFileSourceCode(fileName);

		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			cout << "Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}