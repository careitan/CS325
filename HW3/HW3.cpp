// CS 325 - W2018 Homework 3
// Oregon State University
// (Craig) Allan Reitan
// 
// MakeChange.cpp - Change Making Algorithm program that relies on recursion and Memoization of results from previous iterations.
//
// Helper function for minimum coin count: http://comproguide.blogspot.com/2013/12/minimum-coin-change-problem.html
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
#include <omp.h>

using namespace std;

struct DataSet
{
	int Amount2Change = 0;
	vector<int> DemonimationValues;
};

struct MakeChange
{
	deque<int> Coins;
};

int MAKE_CHANGE(vector<int> V, deque<int> &C, vector<MakeChange> &M, int A);
int GetCoinCount(deque<int> C);
int getMinCoins(vector<int> & denom, int amount);
int GetSubValue(vector<int> V, deque<int> &C, int Idx);

int main()
{
	int A;
	vector<DataSet> Inputs;
	vector<MakeChange> Memo;
	string Line;
	double time0, time1;
	time0 = time1 = 0;

	ifstream inFile;
	ofstream outFile;
	ofstream DataOutFile;

	inFile.open("amount.txt");
	outFile.open("change.txt");
	DataOutFile.open("HW3_Time_data.txt");

	A = 0;

	DataSet InputItem;

		// Process the input file.
	while (getline(inFile, Line))
	{
		Line.shrink_to_fit();
		stringstream iss(Line);

		int TestResult = Line.find(".", 0);

		if (TestResult > 0)
		{
			// line parsed has spaces so it must be values.
			// New DataSet Element

			InputItem.Amount2Change = 0;
			InputItem.DemonimationValues.clear();
			InputItem.DemonimationValues.resize(0);

			string token;

			while(getline(iss, token, ' '))
			{
				int val;
				val = stoi(token);
				InputItem.DemonimationValues.push_back(val);
			}

		}
		else if (TestResult < 0)
		{
			// Assume a single value for the Amount to Change
			iss >> InputItem.Amount2Change;

			// Commit the completed DataSet to the Input Vector.
			Inputs.push_back(InputItem);
		}
		else
		{
			throw runtime_error("Unable to process input value from file.");
		}
	}
	// Close out the Resource.
	inFile.close();

	// Start the process of generating the outputs and calculations.
	DataOutFile << "CS325 - HW3 Make Change Time Measurements outputs\n";
	DataOutFile << "AMOUNT;    MakeChange(sec);    MinCoinCount;\n";

	for (int t = 0; t < (int)Inputs.size(); t++)
	{
		// Initialize objects for the loop iteration.
		Memo.clear();
		Memo.resize(1);
		deque<int> CoinArray;
		time0 = time1 = 0;

		// Populate the default value for Memo
		for (int k = 0; k < (int)Inputs[t].DemonimationValues.size(); k++)
		{
			Memo[0].Coins.push_back(0);
		}

		time0 = omp_get_wtime();
		for (int i = 1; i < Inputs[t].Amount2Change + 1; i++)
		{
			CoinArray.clear();
			MAKE_CHANGE(Inputs[t].DemonimationValues, CoinArray, Memo, i);
			MakeChange MC;
			MC.Coins = CoinArray;
			Memo.push_back(MC);
		}
		time1 = omp_get_wtime();

		// Write the output to the files as required - First the HW File
		for (int k = 0; k < (int)Inputs[t].DemonimationValues.size(); k++)
		{
			outFile << Inputs[t].DemonimationValues[k] << " ";
		}
		outFile << "\n" << Inputs[t].Amount2Change << "\n";
		for (int k = 0; k < (int)Memo[Inputs[t].Amount2Change].Coins.size(); k++)
		{
			outFile << Memo[Inputs[t].Amount2Change].Coins[k] << " ";
		}
		outFile << "\n" << GetCoinCount(Memo[Inputs[t].Amount2Change].Coins) << "\n";

		// Write the output to the files as required - Data Metrics
		DataOutFile << Inputs[t].Amount2Change << ";  " << (time1 - time0) << ";     " << getMinCoins(Inputs[t].DemonimationValues, Inputs[t].Amount2Change) << "\n";

	}

	outFile.close();

	return 0;
}

int MAKE_CHANGE(vector<int> V, deque<int> &C, vector<MakeChange> &M, int A)
{
	int CoinCount = 0;
	// Check for Base Case Solution.
	if (A == 1)
	{
		for (int i = 0; i < (int)V.size(); i++)
		{
			if (i==0)
			{
				C.push_back(1);
			}
			else
			{
				C.push_back(0);
			}
		}
		return 0;
	}
	else if ((GetCoinCount(M[A-1].Coins) > 0 ) 
		&&((int)M.size() == A))
	{
		// First time visiting this level of the Memo
		C = M[A - 1].Coins;
		// Increment the lowest value by 1.
		C[0]++;
		// Check to see if the minimum coin count is less than current coin count.
		if (GetCoinCount(C) > getMinCoins(V, A))
		{
			int Target = getMinCoins(V, A);
			int Test = GetCoinCount(C);
			// There is a more optimal solution to this problem.

			// while (Test >= Target)
			if (Test > Target)
			{
				for (int i = 1; i < (int)C.size(); i++)
				{
					if (V[i] <  (2 * V[i-1])) // Denomination of the current coin is not worth at least two of lesser denomination.
					{
						// Decrement this counter by one, move the value to the baseline.
						C[i]--;
						C[0] += (V[i] / V[0]);
						C[i - 1]++;
						C[0] -= (V[i - 1] / V[0]);
					}

					// Cleanup the amounts below the currently adjusted stack.
					for (int j = 0; j < i; j++)
					{
						if ((C[j] * V[j])>= V[j+1])
						{
							C[j + 1]++;
							C[j] -= V[j + 1];
						}
					}

					// Step up the increment of the next higher value.
					int Test2 = GetSubValue(V, C, i)+1;
					if ((C[i] == 0 || Test2 == V[i])
						&& ((V[i] <= A) || (A % V[i] == 0)))
					{
						int qty = V[i];

						for (int j = 0; j < i; j++)
						{
							int temp = C[j];
							if (qty > 0)
							{
								C[j] = (C[j] * V[j] <= qty) ? 0 : floor((C[j] * V[j]) / qty);
								qty -= (temp - C[j])*V[j];
							}
						}

						C[i]++;
					}
				}
				Test = GetCoinCount(C);
			}

			/*
			int j = V.size()-1;
			while (Q > 0)
			{
				int value = V[j];
				int temp = floor(Q / value);
				C[j] = temp;
				Q -= (temp * value);
				j--;
			}
			// Clearout the bottom of the CoinsArray
			if (j >= 0)
			{
				while (j >= 0)
				{
					C[j] = 0;
					j--;
				}
			}
			*/
		}
	}
	return CoinCount;
}

int GetSubValue(vector<int> V, deque<int> &C, int Idx)
{
	int result = 0;

	for (int i = 0; i <= Idx; i++)
	{
		result += C[i] * V[i];
	}

	return result;
}

int GetCoinCount(deque<int> C)
{
	int result = 0;
	if (!C.empty())
	{
		for (int i = 0; i < (int)C.size(); i++)
		{
			result += C[i];
		}
	}
	return result;
}

int getMinCoins(vector<int> &denom, int amount)
{
	int * counts = new int[amount + 1];
	int coins = 0;
	counts[0] = 0;

	int i, j;
	for (i = 1; i <= amount; i++)
	{
		coins = INT_MAX;

		for (j = 0; j < (int)denom.size(); j++)
		{
			if (denom[j] <= i)//coin value should not exceed the amount itself
			{
				coins = min(coins, counts[i - denom[j]]);
			}
		}

		if (coins < INT_MAX)
			counts[i] = coins + 1;
		else
			counts[i] = INT_MAX;
	}

	coins = counts[amount];
	delete[] counts;
	return coins;
}