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

#include "Global.h"

using namespace std;

// Serial Function Implementation
void serial_Calculate(vector<float> &values)
{
    // Create an instance of the SerialStatistics
    SerialStatistics sStats = SerialStatistics();

    // Check values size
    string message = "";
    if (values.size() == 0)
        message = "No Data";
    else if (values.size() <= 2)
        message = "Insufficient Data";

    if (message != "")
    {
        displayInfo_Summary(values.size(), 0, 0, 0, 0, 0, 0, 0, 0, 0, message);
        return;
    }

    // Start Counting
    clock_t startTime = clock();

    vector<float> temperature = values; // Copy the values to vector

    // Sorting
    sStats.mergeSort(temperature, SORT_ORDER::ASCENDING); // Perform merge sort - Fastest
    // sStats.selectionSort(temperature, SORT_ORDER::ASCENDING); // Perform selection sort
    // sStats.bubbleSort(temperature, SORT_ORDER::ASCENDING);    // Perform bubble sort - Slowest

    // Calculate the variables
    int size = temperature.size();
    float sum = sStats.getSum(temperature);

    float min = temperature[0];
    float max = temperature[size - 1];
    float mean = sum / (size);
    float sDeviation = sStats.getSDeviation(temperature);
    float median = sStats.getMedian(temperature);
    float q1 = sStats.getQ1(temperature);
    float q3 = sStats.getQ3(temperature);

    // End counting
    clock_t endTime = clock();

    // Display Data
    displayInfo_Summary(size, mean, sDeviation, min, max, median, q1, q3, startTime, endTime);
}

// Summary Functions
void serial_Overall(vector<float> &temp)
{
    // Display overall header
    displayInfo_Overall_Header();

    // Start Counting
    clock_t overallStartTime = clock();

    // Calculate and display the temperature data
    cout << "| " << left << setfill(' ') << setw(14) << "OVERALL";
    serial_Calculate(temp);

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);

    return;
}

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
        serial_Calculate(tempVar[i]); // Calculate and display the temperature data
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
            serial_Calculate(tempVar);
            tempVar.clear();

            // Update the current station and add the new temperature
            currentStation = stationName[i];
            tempVar.push_back(temp[i]);
        }
    }

    // Process the last station's data
    cout << "| " << left << setfill(' ') << setw(14) << currentStation;
    serial_Calculate(tempVar);

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
    vector<float> tempData;
    // Step 1. Create a list of all months regardless station with index
    for (int i = 0; i < temp.size(); i++)
    {
        monthData[month[i] - 1].push_back(temp[i]); // Get Temperature Value
        indexMonthData[month[i] - 1].push_back(i);  // Get Index Value
    }

    // Step 2. For each month, collect all data for each station
    for (int i = 0; i < 12; i++)
    {
        displayInfo_TableDiv(' ');
        cout << "| " << left << setfill(' ') << setw(158) << MONTH_LIST[i] << "|" << endl; // Display the month name
        displayInfo_TableDiv(' ');

        unordered_set<string> copiedUniqueStation = uniqueStation; // Copy the unique station
        if (!monthData[i].empty())
        {
            string currentStation = stationName[indexMonthData[i][0]]; // Initialize with the first station name
            vector<float> tempData;

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
                    serial_Calculate(tempData);
                    tempData.clear();
                    copiedUniqueStation.erase(currentStation);

                    // Update the current station and add the new temperature
                    currentStation = stationName[indexMonthData[i][j]];
                    tempData.push_back(temp[indexMonthData[i][j]]);
                }
            }

            // Process the last station's data
            cout << "| " << left << setfill(' ') << setw(14) << currentStation;
            serial_Calculate(tempData);
            copiedUniqueStation.erase(currentStation);
        }

        for (const auto &station : copiedUniqueStation)
        {
            cout << "| " << left << setfill(' ') << setw(14) << station;
            serial_Calculate(tempData);
        }
        if (i != 11)
            displayInfo_TableDiv('-');
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
            displayInfo_TableDiv(' ');
            cout << "| " << left << setfill(' ') << setw(158) << currentStation << "|" << endl; // Display the station name
            displayInfo_TableDiv(' ');
            for (int j = 0; j < tempData.size(); j++)
            {
                cout << "| " << left << setfill(' ') << setw(14) << MONTH_LIST[j];
                serial_Calculate(tempData[j]);
            }
            displayInfo_TableDiv('-');
            tempData.clear();
            tempData.resize(12);

            // Update the current station and add the new temperature
            currentStation = stationName[i];
            tempData[month[i] - 1].push_back(temp[i]);
        }
    }

    displayInfo_TableDiv(' ');
    cout << "| " << left << setfill(' ') << setw(158) << currentStation << "|" << endl; // Display the station name
    displayInfo_TableDiv(' ');
    for (int j = 0; j < tempData.size(); j++)
    {
        cout << "| " << left << setfill(' ') << setw(14) << MONTH_LIST[j];
        serial_Calculate(tempData[j]);
    }

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);
}

void serial_Histogram(vector<float> &temp)
{
    SerialStatistics sStats = SerialStatistics();
    // Step 1. Start Clock
    clock_t startTime = clock();

    // Step 2. Create a copy of sorted temperature and create a vector to store the frequency of each bins
    vector<float> temp = temp;
    vector<float> upper_Limits; // upper limit for each bins
    vector<int> frequencies;    // store frequency of each bins

    // Step 3. Sort the temperature, get the minimum and maximum
    sStats.mergeSort(temp, SORT_ORDER::ASCENDING);
    // sStats.selectionSort(temp, SORT_ORDER::ASCENDING);
    // sStats.bubbleSort(temp, SORT_ORDER::ASCENDING);
    float minimum = temp[0];
    float maximum = temp[temp.size() - 1];
    float binSize = (maximum - minimum) / HISTOGRAM_BIN_NO;

    // Step 4. Create output vector
    vector<int> histogram_vector(HISTOGRAM_BIN_NO); // histogram results

    // Step 5. Display Header
    displayInfo_Histogram_Header(HISTOGRAM_BIN_NO, binSize, minimum, maximum);

    // Step 6. Calculate the histogram
    // Clear the vector and max frequency
    int max_freq = 0;
    upper_Limits.clear();
    frequencies.clear();

    // For each temperature, calculate the frequency
    for (int i = 0; i < temp.size(); i++)
    {
        float compareVal = minimum + binSize;
        int idx = 0;
        while (temp[i] > compareVal)
        {
            compareVal += binSize; // check next range
            idx++;
        }
        if (idx == HISTOGRAM_BIN_NO)
        {
            idx--;
        }
        histogram_vector[idx] += 1;
    }

    // Add the first minimum value
    upper_Limits.push_back(minimum);

    // Step 7. Save the result
    ofstream outputFile(SERIAL_HISTOGRAM_CSV);     // Open the file
    for (int i = 1; i < HISTOGRAM_BIN_NO + 1; i++) // Display the histogram
    {
        float binStart = minimum + ((i - 1) * binSize);
        float binEnd = minimum + (i * binSize);
        int frequency = (histogram_vector[i - 1]);

        displayInfo_Histogram_Summary(binStart, binEnd, frequency);          // Print histogram summary
        outputFile << binStart << ',' << binEnd << ',' << frequency << endl; // Save to file

        max_freq = (frequency > max_freq) ? frequency : max_freq; // Get the maximum frequency
        frequencies.push_back(frequency);
        upper_Limits.push_back(binEnd);
    }
    outputFile.close();              // Close the file
    frequencies.push_back(max_freq); // Add the maximum frequency, last element

    // Step 8. End Clock
    clock_t endTime = clock();

    // Step 9. Display Footer
    displayInfo_Footer(startTime, endTime);
}

void serial_Histogram_By_Month(vector<float> &temp, vector<int> &month)
{
    // Create a 2D vector to store the temperature data by month
    vector<vector<float>> tempVar(12);

    // Step 1. Isolate Temperature data by month
    for (int i = 0; i < temp.size(); i++)
        tempVar[month[i] - 1].push_back(temp[i]); // Insert the temperature to the specific month. [[month][temperature]]

    // Step 2. Calculate for each available month
    for (int i = 0; i < tempVar.size(); i++)
    {
        cout << left << setfill('=') << setw(160) << " " << MONTH_LIST[i];
        serial_Histogram(tempVar[i]); // Calculate and display the temperature data
    }

    return;
}

#endif // SERIAL_OPERATION_H