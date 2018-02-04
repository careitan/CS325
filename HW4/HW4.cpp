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

void FindActivities (vector<Activity> A, vector<Activity> S);
void RenderOutput (vector<Activity> S, int SetNumber);

int main()
{
	int validation;   	// used to hold the value for validation checking of input list.
	int k;				// Counter to use for the output display indicating which Set we are displaying.
	string Line;
	ifstream inFile;

	inFile.open("act.txt");
	vector<Activity> AllActivities;
	vector<Activity> SelectedActivites;

	// Process the input file.
	validation = 0;
	k = 1;
	while (getline(inFile, Line))
	{
		Line.shrink_to_fit();
		stringstream iss(Line);

		int TestResult = Line.find(" ", 0);

		if (TestResult > 0)
		{
			// line parsed has spaces so it must be values.
			// New DataSet Element

		}
		else if (TestResult < 0)
		{
			// Assume a single value for the Amount of Activities
			iss >> validation;
			AllActivities.clear();
			SelectedActivites.clear();
		}
		else
		{
			throw runtime_error("Unable to process input value from file.");
		}

		// Check to see if all of the values have been read into the vector for processing.
		if ( (int)AllActivities.size() == validation )
		{
			//Process the dataset to find the result set.
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

void FindActivities (vector<Activity> A, vector<Activity> S)
{

	return;
}

void RenderOutput (vector<Activity> S, int SetNumber)
{

	return;
}