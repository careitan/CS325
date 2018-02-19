// CS 325 - W2018 Homework 5
// Oregon State University
// (Craig) Allan Reitan
// 
// HW5.cpp - BFS & DFS for Graph Edge determinations.
// 
// useage: HW5 file_name
// 
// Purpose: Read in a set of names and relationships identified in a text file.
// 		Then perform a Breadth First Search or Depth First Search to determine if there are an equal
//		number of rivalries between "babyfaces" and "heels".
//
//		If there is a determined relationship set, then render it out on the screen.
//

#include <stdio.h>
#include <algorithm>
#include <cstring>
#include <math.h>
#include <vector>
#include <deque>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <iterator>
#include <climits>
#include <cctype>

using namespace std;

const int BABYFACE = 0;
const int HEEL = 1;

const int FALSE = 0;
const int TRUE = 1;

struct Relationship
{
	string Babyface;
	string Heel;
};

struct Wrestler
{
	string Name;
	int Type; // INT_MIN = Undefinded; 0 = Babyface; 1 = Heel
};


void RenderOutput(vector<Wrestler>& W);
void AssignWrestlerType(vector<Wrestler>& W, string name, int Type);
int CompareWrestlerType(vector<Wrestler>& W, string name, int Type);
int GetWrestlerType(vector<Wrestler>& W, string name);
void MSTSet(vector<Wrestler>& W, vector<Relationship>& R);

int main(int argc, char* argv[])
{
	if (argc <=1)
	{
		printf("ERROR: no file name provided.\nUSAGE: HW5 file_name\n");
	}

	// Initialize variables for this problem
	int validation;   	// used to hold the value for validation checking of input list.
	//int i;				// Counter for any loops that are used in the program.
	int j;				// Hold the value for how many times the loop needs to iterate.
	string Line;
	ifstream inFile;

	inFile.open(argv[1]);

	// Setup the vector arrays for this problem
	vector<Wrestler> Wrestlers;
	vector<Relationship> Relationships;
	Wrestlers.clear();
	Wrestlers.resize(0);
	Wrestlers.shrink_to_fit();
	Relationships.clear();
	Relationships.resize(0);
	Relationships.shrink_to_fit();


	// Read input file
	while (getline(inFile, Line))
	{
		Line.shrink_to_fit();
		stringstream iss(Line);

		int TestResult = Line.find(" ", 0);

		if (TestResult > 0)
		{
			// line parsed has spaces so it must be Relationship values.
			// New Relationship Set Element

			j = 0;
			string token;
			Relationship TempStruct;

			while(getline(iss, token, ' '))
			{				
				switch(j)
				{
					case 1:
						TempStruct.Heel = token;
						break;
					default:
						TempStruct.Babyface = token;
						break;
				}
				j++;
			}

			// Add TempStruct to the Relationship Set
			Relationships.push_back(TempStruct);
		}
		else if (TestResult < 0)
		{
			// Single value needs to be processed to see if it is an Integer or string.
			string token;
			iss >> token;
			if (isdigit(token[0]))
			{
				// Have a numeic value that can be converted to a number.
				iss >> validation;
			}
			else	
			{
				// We have some string that needs to be processed.
				Wrestler TempWrestler;
				TempWrestler.Name = token;
				TempWrestler.Type = INT_MIN;

				Wrestlers.push_back(TempWrestler);
			}
		}
		else
		{
			throw runtime_error("Unable to process input value from file.");
		}

		// Check to see if all of the values have been read into the vector set.
		if ((int)Wrestlers.size() == validation ||
			(int)Relationships.size() == validation)
		{
			Wrestlers.shrink_to_fit();
			Relationships.shrink_to_fit();
		}
	}
	
	// Process the Vector Set to find solution
	MSTSet(Wrestlers, Relationships);

	RenderOutput(Wrestlers);

	// Close out the Resource.
	inFile.close();
	return 0;
}

//////////////////////////
//	FUNCTION BUILDS
//////////////////////////
void RenderOutput(vector<Wrestler>& W)
{
	int StopSearch = 0;
	int i = 0;

	do
	{
		if (CompareWrestlerType(W, W[i++].Name, INT_MIN)==TRUE)
		{
			StopSearch = 1;
		}
	}while(i < (int)W.size() && StopSearch == 0);

	if (StopSearch == 1)
	{
		// Not possible to match up all of the set of names to relationships.
		printf("Impossible.\n");
	}else
	{
		// Render an Output Set
		printf("Yes possible\n");

		// Print out the Babyface names first
		printf("Babyfaces:");
		for (i = 0; i < (int)W.size(); ++i)
		{
			if (CompareWrestlerType(W, W[i].Name, BABYFACE)==TRUE)
			{
				cout << " " << W[i].Name;
			}
		}

		// Print out the Heels next.
		printf("\nHeels:");
		for (i = 0; i < (int)W.size(); ++i)
		{
			if (CompareWrestlerType(W, W[i].Name, HEEL)==TRUE)
			{
				cout << " " << W[i].Name;
			}
		}
		printf("\n");
	}

	return;
}

// Quick function to be able to assign the wrestler type to a given name.
void AssignWrestlerType(vector<Wrestler>& W, string name, int Type)
{
	int i;

	//Step through to find the name of string.
	for (i = 0; i < (int)W.size(); ++i)
	{
		if (W[i].Name == name)
		{
			// Found the name now going to set the value.
			W[i].Type = Type;

			//immediately exit the function.
			return;
		}
	}

	return;
}

// Compare the given integer value with the wrestler's current Type indicator.
// Return TRUE if it matches.
int CompareWrestlerType(vector<Wrestler>& W, string name, int Type)
{
	int Retval = FALSE;
	int i;

	//Step through to find the name of string.
	for (i = 0; i < (int)W.size(); ++i)
	{
		if (W[i].Name == name)
		{
			// Found the value and can check it against the test value.
			Retval = (W[i].Type == Type)? TRUE : FALSE;
			return Retval;
		}
	}

	return Retval;
}

// Get the wrestler type and return that value as an integer.
int GetWrestlerType(vector<Wrestler>& W, string name)
{
	int Retval = INT_MIN;
	int i;

	//Step through to find the value.
	for (i = 0; i < (int)W.size(); ++i)
	{
		if (W[i].Name == name)
		{
			// Found the value and can check it against the test value.
			Retval = W[i].Type;
			return Retval;
		}
	}

	return Retval;
}

// Implement the Kruksal's alogorithm functionality to process the array
void MSTSet(vector<Wrestler>& W, vector<Relationship>& R)
{
	int RetValB, RetValH;
	int i;

	// Step through the Relationships to assign wrestler types
	for (i = 0; i < (int)R.size(); ++i)
	{
		switch (i)
		{
			case 0:
				// Automatically assign the first relationship in the set.
				AssignWrestlerType(W, R[i].Babyface, BABYFACE);
				AssignWrestlerType(W, R[i].Heel, HEEL);

				break;
			default:
				// Process the relationship based on rules.
				RetValB = GetWrestlerType(W, R[i].Babyface);
				RetValH = GetWrestlerType(W, R[i].Heel);

				if (RetValB == INT_MIN &&
					RetValH == HEEL)
				{
					// Can assign a Babyface to the name
					AssignWrestlerType(W, R[i].Babyface, BABYFACE);
				}else if (RetValB == BABYFACE &&
					RetValH == INT_MIN)
				{
					// Can assign a Heel to the set.
					AssignWrestlerType(W, R[i].Heel, HEEL);
				}else if (RetValB == INT_MIN &&
					RetValH == INT_MIN)
				{
					// Edge does not touch the set.
					AssignWrestlerType(W, R[i].Babyface, BABYFACE);
					AssignWrestlerType(W, R[i].Heel, HEEL);
					
				}
				break;
		}
	}

	return;
}