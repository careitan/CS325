// CS 325 - W2018 Homework 4
// Oregon State University
// (Craig) Allan Reitan
// 
// HW4.cpp - Greedy Algorithms processing of activities counting.
// 
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

using namespace std;

struct Activity
{
	int Number;
	int Start;
	int Finish;
};

// https://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects
struct less_than_finish
{
	inline bool operator() (const Activity& str1, const Activity& str2) const
    {
        return (str1.Finish > str2.Finish);
    }
};

struct less_than_start
{
	inline bool operator() (const Activity& str1, const Activity& str2) const
    {
        return (str1.Start > str2.Start);
    }
};

void FindActivities (vector<Activity>& A, vector<Activity>& S);
void RenderOutput (vector<Activity>& S, int SetNumber);

int main()
{
	int validation;   	// used to hold the value for validation checking of input list.
	int j;				// Counter for which element in the dataset it is processing.
	int k;				// Counter to use for the output display indicating which Set we are displaying.
	string Line;
	ifstream inFile;

	inFile.open("act.txt");
	vector<Activity> AllActivities;
	vector<Activity> SelectedActivites;

	// Process the input file.
	validation = 0;
	k = 0;
	while (getline(inFile, Line))
	{
		Line.shrink_to_fit();
		stringstream iss(Line);

		int TestResult = Line.find(" ", 0);

		if (TestResult > 0)
		{
			// line parsed has spaces so it must be values.
			// New DataSet Element

			j = 0;
			int a, b, c;
			string token;

			while(getline(iss, token, ' '))
			{
				int val;
				
				val = stoi(token);
				
				switch(j)
				{
					case 1:
						b = val;
						break;
					case 2:
						c = val;
						break;
					default:
						a = val;
						break;
				}
				j++;
			}
			// Populate TempStruct
			Activity TempStruct;
			TempStruct.Number = a;
			TempStruct.Start = b;
			TempStruct.Finish = c;

			// Add TempStruct to the Input Set
			AllActivities.push_back(TempStruct);
		}
		else if (TestResult < 0)
		{
			// Assume a single value for the Amount of Activities
			iss >> validation;
			k++;
			AllActivities.clear();
			AllActivities.resize(0);
			AllActivities.shrink_to_fit();
			SelectedActivites.clear();
			SelectedActivites.resize(0);
			SelectedActivites.shrink_to_fit();
		}
		else
		{
			throw runtime_error("Unable to process input value from file.");
		}

		// Check to see if all of the values have been read into the vector for processing.
		if ( (int)AllActivities.size() == validation )
		{
			//Process the dataset to find the result set.
			AllActivities.shrink_to_fit();
			FindActivities (AllActivities, SelectedActivites);

			//Print out the result set to the terminal.
			RenderOutput (SelectedActivites, k);
		}

	}
	// Close out the Resource.
	inFile.close();

	// Start the process of generating the outputs and calculations.

	return 0;
}

//////////////////////////
//	FUNCTION BUILDS
//////////////////////////

void FindActivities (vector<Activity>& A, vector<Activity>& S)
{
	// plaseholder for the curent value of Start.
	int StartingTime = INT_MAX;

	S.clear();

	stable_sort(A.begin(), A.end(), less_than_finish());
	stable_sort(A.begin(), A.end(), less_than_start());

	for (int i = 0; i < (int)A.size(); ++i)
	{
		if (A[i].Finish <= StartingTime)
		{
			S.insert(S.begin(), A[i]);
			StartingTime = A[i].Start;
		}
	}

	return;
}

void RenderOutput (vector<Activity>& S, int SetNumber)
{
	printf("\nSet %i\n", SetNumber);
	printf("Number of activities selected = %i\n",(int)S.size());
	printf("Activities: ");

	for (int i = 0; i < (int)S.size(); ++i)
	{
		printf("%i ", S[i].Number);
	}
	printf("\n");

	return;
}