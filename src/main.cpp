#include <iostream>
#include <exception>

#include "SyntaxAnalysis.h"

using namespace std;

/**
*string fin - file name to read lines from
* 
*Function prints all lines to terminal of the input file
*/
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

		if (lex.Do())
		{
			cout << "Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}

		SyntaxAnalysis syl(lex);
		cout << "Syntax analysis started..." << endl;
		if (syl.Do())
		{
			cout << "Syntax analysis finished successfully!" << endl;
		}
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}