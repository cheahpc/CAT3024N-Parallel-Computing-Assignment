#ifndef SERIAL_STATISTICS_H
#define SERIAL_STATISTICS_H

// Includes
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Sort Type ENUM
typedef enum SERIAL_SORT_ORDER
{
	ASCENDING,
	DESCENDING
};

class SerialStatistics
{
public:
	// Empty Constructor and Destructor
	SerialStatistics();
	~SerialStatistics();

	// Function Declarations
	// Sort Functions
	void SerialStatistics::bubbleSort(vector<float> &values, SERIAL_SORT_ORDER mode);
	void SerialStatistics::selectionSort(vector<float> &values, SERIAL_SORT_ORDER mode);
	void SerialStatistics::mergeSort(vector<float> &values, SERIAL_SORT_ORDER mode);
	void SerialStatistics::merge(vector<float> &values, const vector<float> &left, const vector<float> &right, SERIAL_SORT_ORDER mode);

	void SerialStatistics::Display(vector<float> &values); // Displays a vector

	// Statistics Functions
	float SerialStatistics::getSum(vector<float> &values);
	float SerialStatistics::getMinMax(vector<float> &values, bool getMin);
	float SerialStatistics::getMedian(vector<float> &values);
	float SerialStatistics::getMean(vector<float> &values);
	float SerialStatistics::getSDeviation(vector<float> &values);
	float SerialStatistics::getQ1(vector<float> &values);
	float SerialStatistics::getQ3(vector<float> &values);
};

#endif // SERIAL_STATISTICS_H