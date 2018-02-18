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


void RenderOutput();
int CompareWrestlerType(vector<Wrestler>& w, string name, int Type);

int main(int argc, char* argv[])
{
	if (argc <=1)
	{
		printf("ERROR: no file name provided.\nUSAGE: HW5 file_name\n");
	}

	return 0;
}

//////////////////////////
//	FUNCTION BUILDS
//////////////////////////
void RenderOutput()
{

	return;
}

// Compare the 
int CompareWrestlerType(vector<Wrestler>& w, string name, int Type)
{
	int Retval = TRUE;



	return Retval;
}