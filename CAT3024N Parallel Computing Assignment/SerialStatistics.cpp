#include "SerialStatistics.h"

using namespace std;
// Empty Constructor
SerialStatistics::SerialStatistics()
{
}

// Empty Destructor
SerialStatistics::~SerialStatistics()
{
}

void SerialStatistics::bubbleSort(vector<float> &values, SERIAL_SORT_ORDER mode)
{
	// Get the size of the input vector
	int Size = values.size();
	float temp;
	// Loop through performing bubble sort
	if (mode == ASCENDING)
	{
		for (int i = 0; i < Size; i++)
		{
			for (int j = 0; j < Size - 1; j++)
			{
				// If the next element is less than the current element
				if (values[j + 1] < values[j])
				{
					// Swap the two values
					temp = values[j];
					values[j] = values[j + 1];
					values[j + 1] = temp;
				}
			}
		}
	}
	else if (mode == DESCENDING)
	{
		for (int i = 0; i < Size; i++)
		{
			for (int j = 0; j < Size - 1; j++)
			{
				// If the next element is greater than the current element
				if (values[j + 1] > values[j])
				{
					// Swap the two values
					temp = values[j];
					values[j] = values[j + 1];
					values[j + 1] = temp;
				}
			}
		}
	}
	return;
}

void SerialStatistics::selectionSort(vector<float> &values, SERIAL_SORT_ORDER mode)
{
	int min_index;
	if (SERIAL_SORT_ORDER::ASCENDING)
	{
		// One by one move boundary of unsorted array
		for (int i = 0; i < values.size() - 1; i++)
		{
			min_index = i;
			for (int j = i + 1; j < values.size(); j++)
			{
				// Store the index of minimum element in unsorted array
				if (values[j] < values[min_index])
				{
					min_index = j;
				}
			}
			// Swap the found min element with the bigger element
			if (min_index != i)
			{
				// Sorting or Swapping elements
				float temp = values[min_index]; // Put the min value into temporary var
				values[min_index] = values[i];	// Put the bigger element to the min value's element array
				values[i] = temp;				// Put the min element to go into the bigger element array
			}
		}
	}
	else if (SERIAL_SORT_ORDER::DESCENDING)
	{
		// One by one move boundary of unsorted array
		for (int i = 0; i < values.size() - 1; i++)
		{
			min_index = i;
			for (int j = i + 1; j < values.size(); j++)
			{
				// Store the index of minimum element in unsorted array
				if (values[j] > values[min_index])
				{
					min_index = j;
				}
			}
			// Swap the found min element with the bigger element
			if (min_index != i)
			{
				// Sorting or Swapping elements
				float temp = values[min_index]; // Put the min value into temporary var
				values[min_index] = values[i];	// Put the bigger element to the min value's element array
				values[i] = temp;				// Put the min element to go into the bigger element array
			}
		}
	}

	return;
}

void SerialStatistics::mergeSort(vector<float> &values, SERIAL_SORT_ORDER mode)
{
	// Base case: array of size 1 or less is already sorted
	if (values.size() <= 1)
	{
		return;
	}

	// Set the middle index
	int mid = values.size() / 2;

	// Divide the array into two halves
	vector<float> left(values.begin(), values.begin() + mid);
	vector<float> right(values.begin() + mid, values.end());

	// Recursively sort the two halves
	mergeSort(left, mode);
	mergeSort(right, mode);

	// Merge the sorted halves
	merge(values, left, right, mode);

	return;
}

void SerialStatistics::merge(vector<float> &values, const vector<float> &left, const vector<float> &right, SERIAL_SORT_ORDER mode)
{
	int i = 0, j = 0, k = 0; // i = left index, j = right index, k = merged index

	if (mode == ASCENDING)
	{
		while (i < left.size() && j < right.size())
		{
			if (left[i] < right[j])
			{
				values[k++] = left[i++];
			}
			else
			{
				values[k++] = right[j++];
			}
		}
	}
	else if (mode == DESCENDING)
	{
		while (i < left.size() && j < right.size())
		{
			if (left[i] > right[j])
			{
				values[k++] = left[i++];
			}
			else
			{
				values[k++] = right[j++];
			}
		}
	}

	// Copy the remaining elements of left, if there are any
	while (i < left.size())
	{
		values[k++] = left[i++];
	}

	// Copy the remaining elements of right, if there are any
	while (j < right.size())
	{
		values[k++] = right[j++];
	}

	return;
}

float SerialStatistics::getSum(vector<float> &values)
{
	// Init the sum holder
	float Sum = 0;
	// Get the size of the input vector
	int Size = values.size();
	// Loop through the vector
	for (int i = 0; i < Size; i++)
	{
		// Add the current element to the sum
		Sum += values[i];
	}
	// Return the sum
	return Sum;
}

float SerialStatistics::getMinMax(vector<float> &values, bool getMin = true)
{
	// Init the min or max variable used to store either the current min or max value
	float val = 0;
	// Bool to check first set
	bool FirstSet = false;
	// Get the size of the input vector
	int Size = values.size();
	// Loop through the vector
	for (int i = 0; i < Size; i++)
	{
		if (((val > values[i]) && (getMin == true)) || (FirstSet == false))
		{
			// Store smaller value
			val = values[i];
			// Set firstset to true
			FirstSet = true;
		}

		if (((val < values[i]) && (getMin == false)) || (FirstSet == false))
		{
			// Store greater value
			val = values[i];
			// Set firstset to true
			FirstSet = true;
		}
	}
	// Return either the min or max value depending on MinMax
	return val;
}

void SerialStatistics::Display(vector<float> &values)
{
	// Get the size of the input vector
	int Size = values.size();
	// Loop through the vector
	for (int i = 0; i < Size; i++)
	{
		// Output the current element of the vector
		cout << values[i] << " : ";
	}
	// Add end line to console
	cout << endl;
}

float SerialStatistics::getMean(vector<float> &values)
{
	// Get the sum of the input vector
	float sum = getSum(values);
	// Get the size of the input vector
	int Size = values.size();
	// Divide the sum of the vector by its size to get the mean
	float Mean = sum / Size;
	// Return the mean value
	return Mean;
}

float SerialStatistics::getSDeviation(vector<float> &values)
{
	// Define three variables to hold values
	float sum = 0, mean, standardDeviation = 0.0;
	// Calculate the mean of the vector
	mean = getMean(values);
	// Get the size of the input vector
	int Size = values.size();
	// Loop through the vector
	for (int i = 0; i < Size; i++)
	{
		// Calculate the first part of standard deviation
		standardDeviation += ((values[i] - mean) * (values[i] - mean));
	}
	// Divide the standard deviation by its size
	standardDeviation = (standardDeviation / (Size));
	// Return the square root of the current value stored
	return sqrt(standardDeviation);
}

float SerialStatistics::getMedian(vector<float> &values)
{
	// Get the size of the input vector
	int Size = values.size();
	// Variable to hold middle value
	float Middle;

	// Check if size is an even number
	if (Size % 2 == 0)
	{
		// Calculate the difference between the two middle values
		float Difference = values[(Size / 2) - 1] - values[(Size / 2)];
		// Account for difference to find the true median value
		Middle = values[(Size / 2) - 1] - Difference / 2;
	}
	else
	{
		Middle = values[(Size / 2)]; // Get center element
	}

	return Middle;
}

float SerialStatistics::getQ1(vector<float> &values)
{
	// Get the size of the input vector
	int Size = values.size();
	// Variable to hold the first quartile value
	float First;
	// Check if size is an even number
	if (Size % 2 == 0)
	{
		// Calculate the difference between the two first quartile values
		float Difference = values[(Size / 4) - 1] - values[(Size / 4)];
		// Account for difference to find the true first quartile value
		First = values[(Size / 4) - 1] - Difference / 2;
	}
	else
	{
		// Get the value of the first quartile
		First = values[(Size / 4)];
	}

	return First;
}

float SerialStatistics::getQ3(vector<float> &values)
{
	// Get the size of the input vector
	int Size = values.size();
	// Variable to hold the third quartile value
	float Third;
	// Set pos to the position of the third quartile
	int Pos = Size - (Size / 4);
	// Check if size is an even number
	if (Size % 2 == 0)
	{
		// Calculate the difference between the two third quartile values
		float Difference = values[Pos - 1] - values[Pos];
		// Account for difference to find the true third quartile value
		Third = values[Pos - 1] - Difference / 2;
	}
	else
	{
		// Get the value of the third quartile
		Third = values[Pos - 1];
	}

	return Third;
}