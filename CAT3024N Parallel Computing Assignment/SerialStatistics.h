#pragma once

// Includes
#include <iostream>
#include <string>
#include <vector>

// Sort Type ENUM
typedef enum SORT
{
	ASCENDING,
	DECENDING
};

class SerialStatistics
{
public:
	// Empty Constructor and Destructor
	SerialStatistics();
	~SerialStatistics();

	// Sorts a input vector using the bubble sort algorithm
	std::vector<float> &SerialStatistics::Sort(std::vector<float> &Values, SORT Mode);
	// Finds the sum of an input vector
	float SerialStatistics::Sum(std::vector<float> &Values);
	// Finds the Min or Max of an input vector
	float SerialStatistics::MinMax(std::vector<float> &Values, bool MinMax);
	// Displays a vector
	void SerialStatistics::Display(std::vector<float> &Values);
	// Gets the median value from an input vector
	float SerialStatistics::GetMedianValue(std::vector<float> &Values);
	// Gets the mean value from an input vector
	float SerialStatistics::Mean(std::vector<float> &Values);
	// Gets the Standard Deviation value from an input vector
	float SerialStatistics::StandardDeviation(std::vector<float> &Values);
	// Gets the First Quartile value from an input vector
	float SerialStatistics::FirstQuartile(std::vector<float> &Values);
	// Gets the Third Quartile value from an input vector
	float SerialStatistics::ThirdQuartile(std::vector<float> &Values);
};