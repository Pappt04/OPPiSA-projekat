/*Author: Papp Tamas Index: RA004/2022 Datum: 2024.06.05*/
#pragma once
#include <string>


class Labels
{
public:
	std::string name;
	int position;

	/*
	* Constructor for creating Labels from the input file with name and position
	*/
	Labels(std::string n, int pos);
};

