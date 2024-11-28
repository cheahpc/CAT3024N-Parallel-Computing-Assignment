#ifndef SERIAL_OPERATION_H
#define SERIAL_OPERATION_H

// Includes
#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <map>
// Custom Includes
#include "SerialStatistics.h"
#include "Display.h"
#include "Sort.h"

using namespace std;

// Constant Variables
const string MONTH_LIST[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Serial Function Implementation
void serial_Calculate(vector<float> &values, bool isOverall = true)
{
    // Create an instance of the SerialStatistics
    SerialStatistics SStats = SerialStatistics();

    // Check values size
    string message = "";
    if (values.size() == 0)
        message = "No Data";
    else if (values.size() <= 2)
        message = "Insufficient Data";

    if (message != "")
    {
        displayInfo_Summary(isOverall, values.size(), 0, 0, 0, 0, 0, 0, 0, 0, 0, message);
        return;
    }

    // Start Counting
    clock_t startTime = clock();

    vector<float> temperature = values; // Copy the values to vector

    // Sorting
    SStats.mergeSort(temperature, SORT_ORDER::ASCENDING); // Perform merge sort - Fastest
    // SStats.selectionSort(temperature, SORT_ORDER::ASCENDING); // Perform selection sort
    // SStats.bubbleSort(temperature, SORT_ORDER::ASCENDING);    // Perform bubble sort - Slowest

    // Calculate the variables
    int size = temperature.size();
    float sum = SStats.getSum(temperature);

    float min = temperature[0];
    float max = temperature[size - 1];
    float mean = sum / (size);
    float sDeviation = SStats.getSDeviation(temperature);
    float median = SStats.getMedian(temperature);
    float q1 = SStats.getQ1(temperature);
    float q2 = SStats.getQ3(temperature);

    // End counting
    clock_t endTime = clock();

    // Display Data
    displayInfo_Summary(isOverall, size, mean, sDeviation, min, max, median, q1, q2, startTime, endTime);
}

// Summary Functions
void serial_By_Month(vector<float> &temp, vector<int> &month)
{
    // Display by month header
    displayInfo_By_Month_Header();

    // Start Counting
    clock_t overallStartTime = clock();

    // Create a 2D vector to store the temperature data by month
    vector<vector<float>> tempVar(12);

    // Step 1. Isolate Temperature data by month
    for (int i = 0; i < temp.size(); i++)
        tempVar[month[i] - 1].push_back(temp[i]); // Insert the temperature to the specific month. [[month][temperature]]

    // Step 2. Calculate for each available month
    for (int i = 0; i < tempVar.size(); i++)
    {
        cout << "| " << left << setfill(' ') << setw(14) << MONTH_LIST[i];
        serial_Calculate(tempVar[i], false); // Calculate and display the temperature data
    }

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);

    return;
}

void serial_By_Station(vector<float> &temp, vector<string> &stationName)
{
    // Display by station header
    displayInfo_By_Station_Header();

    // Start Counting
    clock_t overallStartTime = clock();

    // Create a 1D vector to store the temperature data by station
    vector<float> tempVar;
    string currentStation = stationName[0]; // Initialize with the first station name

    for (int i = 0; i < temp.size(); i++)
    {
        if (stationName[i] == currentStation)
        {
            tempVar.push_back(temp[i]);
        }
        else
        {
            // Print and process the current station's data
            cout << "| " << left << setfill(' ') << setw(14) << currentStation;
            serial_Calculate(tempVar, false);
            tempVar.clear();

            // Update the current station and add the new temperature
            currentStation = stationName[i];
            tempVar.push_back(temp[i]);
        }
    }

    // Process the last station's data
    cout << "| " << left << setfill(' ') << setw(14) << currentStation;
    serial_Calculate(tempVar, false);

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);

    return;
}

void serial_By_Month_All_Station(vector<float> &temp, vector<string> &stationName, vector<int> &month)
{
    // Each month all station
    displayInfo_By_Station_Header();

    // Start Counting
    clock_t overallStartTime = clock();

    vector<vector<float>> monthData(12); // For each month, store the temperature data for each station
    vector<vector<int>> indexMonthData(12);
    vector<vector<vector<float>>> station;

    int currentStation = 0;

    vector<float> currentMonthData;

    // Step 1. create a list of all months regardless station with index
    for (int i = 0; i < temp.size(); i++)
    {
        monthData[month[i] - 1].push_back(temp[i]); // Get Temperature Value
        indexMonthData[month[i] - 1].push_back(i);  // Get Index Value
    }

    cout << "Checkpoint: ";

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);
}

void serial_By_Station_All_Month(vector<float> &temp, vector<string> &stationName, vector<int> &month)
{
    // Each station all month
    displayInfo_By_Month_Header();

    // Start Counting
    clock_t overallStartTime = clock();

    // Step 1. Collect data for each station
    vector<vector<float>> tempData(12);     // For each station, store the temperature data for each month
    string currentStation = stationName[0]; // Initialize with the first station name

    // Each station
    for (int i = 0; i < temp.size(); i++)
    {
        if (stationName[i] == currentStation)
        {
            tempData[month[i] - 1].push_back(temp[i]);
        }
        else
        {
            // Print and process the current station's data
            cout << "|" << internal << setfill(' ') << setw(160) << "|" << endl;                // Padding
            cout << "| " << left << setfill(' ') << setw(158) << currentStation << "|" << endl; // Display the station name
            cout << "|" << internal << setfill(' ') << setw(160) << "|" << endl;                // Padding
            for (int j = 0; j < tempData.size(); j++)
            {
                cout << "| " << left << setfill(' ') << setw(14) << MONTH_LIST[j];
                serial_Calculate(tempData[j], false);
            }
            cout << "|" << internal << setfill('-') << setw(160) << "|" << endl; // Padding end
            tempData.clear();
            tempData.resize(12);

            // Update the current station and add the new temperature
            currentStation = stationName[i];
            tempData[month[i] - 1].push_back(temp[i]);
        }
    }

    // Process the last station's data
    cout << "| " << left << setfill(' ') << setw(158) << currentStation << "|" << endl; // Display the station name
    cout << "|" << internal << setfill(' ') << setw(160) << "|" << endl;
    for (int j = 0; j < tempData.size(); j++)
    {
        cout << "| " << left << setfill(' ') << setw(14) << MONTH_LIST[j];
        serial_Calculate(tempData[j], false);
    }

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);
}

#endif // SERIAL_OPERATION_H