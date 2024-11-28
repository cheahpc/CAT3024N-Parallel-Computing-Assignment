#ifndef SERIAL_OPERATION_H
#define SERIAL_OPERATION_H

// Includes
#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <map>
#include <unordered_set>
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

    // Create array of unique station
    unordered_set<string> uniqueStation(stationName.begin(), stationName.end());

    vector<vector<float>> monthData(12);    // For each month, store the temperature data for each station
    vector<vector<int>> indexMonthData(12); // For each month, store the index of the temperature data
    vector<float> tempData;                 // Store the temperature data for each station

    string currentStation;

    // Step 1. Create a list of all months regardless station with index
    for (int i = 0; i < temp.size(); i++)
    {
        monthData[month[i] - 1].push_back(temp[i]); // Get Temperature Value
        indexMonthData[month[i] - 1].push_back(i);  // Get Index Value
    }

    // Step 2. For each month, collect all data for each station
    for (int i = 0; i < monthData.size(); i++)
    {
        cout << "|" << internal << setfill(' ') << setw(160) << "|" << endl;               // Padding
        cout << "| " << left << setfill(' ') << setw(158) << MONTH_LIST[i] << "|" << endl; // Display the month name
        cout << "|" << internal << setfill(' ') << setw(160) << "|" << endl;

        unordered_set<string> copiedUniqueStation = uniqueStation; // Copy the unique station
        if (monthData[i].size() > 0)
        {
            currentStation = stationName[indexMonthData[i][0]]; // Initialize with the first station name
            tempData.clear();

            for (int j = 0; j < indexMonthData[i].size(); j++)
            {
                if (stationName[indexMonthData[i][j]] == currentStation)
                {
                    tempData.push_back(temp[indexMonthData[i][j]]);
                }
                else
                {
                    // Print and process the current station's data
                    cout << "| " << left << setfill(' ') << setw(14) << currentStation;
                    serial_Calculate(tempData, false);
                    tempData.clear();
                    copiedUniqueStation.erase(currentStation);

                    // Update the current station and add the new temperature
                    currentStation = stationName[indexMonthData[i][j]];
                    tempData.push_back(temp[indexMonthData[i][j]]);
                }
            }

            // Process the last station's data
            cout << "| " << left << setfill(' ') << setw(14) << currentStation;
            serial_Calculate(tempData, false);
            tempData.clear();
            copiedUniqueStation.erase(currentStation);
        }

        for (auto it = copiedUniqueStation.begin(); it != copiedUniqueStation.end(); it++)
        {
            currentStation = *it; // Get the current station name
            cout << "| " << left << setfill(' ') << setw(14) << currentStation;
            serial_Calculate(tempData, false);
        }
        if (i != monthData.size() - 1)
            cout << "|" << internal << setfill('-') << setw(160) << "|" << endl; // Padding end
    }

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

    cout << "|" << internal << setfill(' ') << setw(160) << "|" << endl;                // Padding// Process the last station's data
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