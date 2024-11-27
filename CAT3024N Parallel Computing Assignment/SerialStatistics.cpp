#include "SerialStatistics.h"

//Empty Constructor
SerialStatistics::SerialStatistics()
{
}

//Empty Destructor
SerialStatistics::~SerialStatistics()
{
}

std::vector<float>& SerialStatistics::Sort(std::vector<float>& Values, SORT Mode)
{
	//Get the size of the input vector
	int Size = Values.size();
	float temp;

	//Loop through performing bubble sort
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size - 1; j++)
		{
			//If DECENDING check if the next element is greater than the current element
			if ((Values[j + 1] > Values[j]) && (Mode == SORT::DECENDING))
			{
				//Swap the two values
				temp = Values[j];
				Values[j] = Values[j + 1];
				Values[j + 1] = temp;
			}
			//If ASCENDING check if the next element is less than the current element
			if ((Values[j + 1] < Values[j]) && (Mode == SORT::ASCENDING))
			{
				//Swap the two values
				temp = Values[j];
				Values[j] = Values[j + 1];
				Values[j + 1] = temp;
			}
		}
	}
	//Return the sorted vector
	return Values;
}

float SerialStatistics::Sum(std::vector<float>& Values)
{
	//Init the sum holder
	float Sum = 0;
	//Get the size of the input vector
	int Size = Values.size();
	//Loop through the vector
	for (int i = 0; i < Size; i++)
	{
		//Add the current element to the sum
		Sum += Values[i];
	}
	//Return the sum
	return Sum;
}

float SerialStatistics::MinMax(std::vector<float>& Values, bool MinMax)
{
	//TRUE = MIN:::::FALSE = MAX
	//Init the min or max variable used to store either the current min or max value
	float MiMx = 0;
	//Bool to check first set
	bool FirstSet = false;
	//Get the size of the input vector
	int Size = Values.size();
	//Loop through the vector
	for (int i = 0; i < Size; i++)
	{
		//If MinMax is true then find if the current value is less than than the stored.
		if (((MiMx > Values[i]) && (MinMax == true)) || (FirstSet == false))
		{
			//Store smaller value
			MiMx = Values[i];
			//Set firstset to true
			FirstSet = true;
		}
		//If MinMax is false then find if the current value is greater than than the stored.
		if (((MiMx < Values[i]) && (MinMax == false)) || (FirstSet == false))
		{
			//Store greater value
			MiMx = Values[i];
			//Set firstset to true
			FirstSet = true;
		}
	}
	//Return either the min or max value depending on MinMax
	return MiMx;
}

void SerialStatistics::Display(std::vector<float>& Values)
{
	//Get the size of the input vector
	int Size = Values.size();
	//Loop through the vector
	for (int i = 0; i < Size; i++)
	{
		//Output the current element of the vector
		std::cout << Values[i] << " : ";
	}
	//Add end line to console
	std::cout << std::endl;
}

float SerialStatistics::Mean(std::vector<float>& Values)
{
	//Get the sum of the input vector
	float sum = Sum(Values);
	//Get the size of the input vector
	int Size = Values.size();
	//Divide the sum of the vector by its size to get the mean
	float Mean = sum / Size;
	//Return the mean value
	return Mean;
}

float SerialStatistics::StandardDeviation(std::vector<float>& Values)
{
	//Define three variables to hold values
	float sum = 0, mean, standardDeviation = 0.0;
	//Calculate the mean of the vector
	mean = Mean(Values);
	//Get the size of the input vector
	int Size = Values.size();
	//Loop through the vector
	for (int i = 0; i < Size; i++)
	{
		//Calculate the first part of standard deviation
		standardDeviation += ((Values[i] - mean)*(Values[i] - mean));
	}
	//Divide the standard deviation by its size
	standardDeviation = (standardDeviation / (Size));
	//Return the square root of the current value stored
	return sqrt(standardDeviation);
}

float SerialStatistics::GetMedianValue(std::vector<float>& Values)
{
	//Get the size of the input vector
	int Size = Values.size();
	//Variable to hold middle value
	float Middle;
	//Check if size is an even number
	if (Size % 2 == 0)
	{
		//Calculate the difference between the two middle values
		float Difference = Values[(Size / 2) - 1] - Values[(Size / 2)];
		//Account for difference to find the true median value
		Middle = Values[(Size / 2) - 1] - Difference / 2;
	}
	else
	{
		//Get center element
		Middle = Values[(Size / 2)];
	}
	//Return median value
	return Middle;
}

float SerialStatistics::FirstQuartile(std::vector<float>& Values)
{
	//Get the size of the input vector
	int Size = Values.size();
	//Variable to hold the first quartile value
	float First;
	//Check if size is an even number
	if (Size % 2 == 0)
	{
		//Calculate the difference between the two first quartile values
		float Difference = Values[(Size / 4) - 1] - Values[(Size / 4)];
		//Account for difference to find the true first quartile value
		First = Values[(Size / 4) - 1] - Difference / 2;
	}
	else
	{
		//Get the value of the first quartile
		First = Values[(Size / 4)];
	}
	//Return the found value
	return First;
}

float SerialStatistics::ThirdQuartile(std::vector<float>& Values)
{
	//Get the size of the input vector
	int Size = Values.size();
	//Variable to hold the third quartile value
	float Third;
	//Set pos to the position of the third quartile
	int Pos = Size - (Size / 4);
	//Check if size is an even number
	if (Size % 2 == 0)
	{
		//Calculate the difference between the two third quartile values
		float Difference = Values[Pos - 1] - Values[Pos];
		//Account for difference to find the true third quartile value
		Third = Values[Pos - 1] - Difference / 2;
	}
	else
	{
		//Get the value of the third quartile
		Third = Values[Pos - 1];
	}
	//Return the found value
	return Third;
}