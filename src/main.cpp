#include <exception>
#include <iostream>

#include "SyntaxAnalysis.h"

#include <Windows.h>

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

		//Set console color for error or successfull operation
		HANDLE console_color;
		console_color = GetStdHandle(STD_OUTPUT_HANDLE);

		LexicalAnalysis lex;

		printOutInputFileSourceCode(fileName);

		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		if (lex.Do())
		{
			cout << "Lexical analysis finished";
			SetConsoleTextAttribute(console_color, 10);
			cout << " successfully";
			SetConsoleTextAttribute(console_color, 15);
			cout << "!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}

		//---------------------------------------------------------------------

		Instructions instructions;

		SyntaxAnalysis syl(lex,instructions);
		cout << endl << "Syntax analysis started..." << endl << endl;
		if (syl.Do())
		{
			cout << "Syntax analysis finished";
			SetConsoleTextAttribute(console_color, 10);
			cout << " successfully";
			SetConsoleTextAttribute(console_color, 15);
			cout << "!" << endl;
		}
		else
		{
			cout << "Syntax analysis finished with";
			SetConsoleTextAttribute(console_color, 12);
			cout << " errors";
			SetConsoleTextAttribute(console_color, 15);
			cout << "!" << endl;
		}
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}
	return 0;
}