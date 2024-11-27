#ifndef SERIAL_OPERATION_H
#define SERIAL_OPERATION_H

// Includes
#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <cmath>
// Custom Includes
#include "SerialStatistics.h"
#include "Display.h"
#include "Sort.h"

// Global control variables

using namespace std;

// Constant Variables
const string MONTH_LIST[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Serial Function Implementation
void serial_Calculate(vector<float> &values, bool isOverall = true)
{
    // Create an instance of the SerialStatistics
    SerialStatistics SStats = SerialStatistics();

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
    if (isOverall)
        displayInfo_Overall(size, mean, sDeviation, min, max, median, q1, q2, startTime, endTime);
    else
        displayInfo_Overall(size, mean, sDeviation, min, max, median, q1, q2, startTime, endTime);
}

void serial_by_month_summary(vector<float> &temp, vector<int> &month)
{
    // Create an instance of the SerialStatistics
    SerialStatistics SStats = SerialStatistics();

    // Start Counting
    clock_t startTime = clock();

    // Display the results for 12 months
    cout << "=================================[MONTHS RESULT]====================================" << std::endl;
    cout << "MONTH    \tMIN \tMAX \tMEAN \tSD \tMEDIAN \t1Q \t3Q \tSIZE \tTIME" << std::endl;
    cout << "====================================================================================" << std::endl;

    // Prepare the 2D vector for the temperature
    vector<vector<float>> temp2D(12);

    // Loop through all the temperature data
    for (int i = 0; i < temp.size(); i++)
    {
        temp2D[month[i] - 1].insert(temp2D[month[i] - 1].begin(), temp[i]);
    }
    // Loop through all the month vector
    for (int i = 0; i < 12; i++)
    {
        cout << MONTH_LIST[i] << "\t\t";
        // Trigger the serial function to display
        serial_Calculate(temp2D[i]);
    }

    // End Counting
    clock_t endTime = clock();

    cout << std::endl;
    cout << "TOTAL COMPLETION TIME: \t" << (endTime - startTime) << " ms" << std::endl
         << std::endl
         << std::endl;
    ;
}

void Serial_Summary(vector<float> &temp, vector<string> &stationName, vector<int> &month)
{

    // Start counting for Station
    float startTime = clock();

    // Display results for stations
    cout << "=================================[STATION RESULT]===================================" << std::endl;
    cout << "STATION    \tMIN \tMAX \tMEAN \tSD \tMEDIAN \t1Q \t3Q \tSIZE \tTIME" << std::endl;
    cout << "====================================================================================" << std::endl;

    // Part of temperature belong to a specific station
    vector<float> partTemp;
    for (int i = 0; i < temp.size(); i++)
    {
        // Is an empty vector
        if (partTemp.size() == 0)
        {
            partTemp.insert(partTemp.begin(), temp[i]);
        }
        else
        {
            // Check if the next station is matched with the current station
            if ((i + 1) != temp.size())
            { // if not last data
                // if matched, continue adding the temperature to partTemp
                if (stationName[i] == stationName[i + 1])
                {
                    partTemp.insert(partTemp.begin(), temp[i]);
                }
                else
                {
                    partTemp.insert(partTemp.begin(), temp[i]);
                    cout << stationName[i] << "  \t";
                    serial_Calculate(partTemp);
                    partTemp.clear(); // Reset the partTemp
                }
            }
            else
            {
                // Last temperature data
                partTemp.insert(partTemp.begin(), temp[i]);
                cout << stationName[i] << " \t\t";
                serial_Calculate(partTemp);
                partTemp.clear(); // Reset the partTemp
            }
        }
    }

    // End Counting for Station
    float endTime = clock();

    cout << std::endl;
    cout << "TOTAL COMPLETION TIME: \t" << (endTime - startTime) << " ms" << std::endl
         << std::endl
         << std::endl;
    ;

    // Start Counting for Months
    startTime = clock();

    // Display the results for 12 months
    cout << "=================================[MONTHS RESULT]====================================" << std::endl;
    cout << "MONTH    \tMIN \tMAX \tMEAN \tSD \tMEDIAN \t1Q \t3Q \tSIZE \tTIME" << std::endl;
    cout << "====================================================================================" << std::endl;

    // Part of temperature belong to a specific month
    vector<vector<float>> temp2D(12);

    // Loop through all the temperature data
    for (int i = 0; i < temp.size(); i++)
    {
        temp2D[month[i] - 1].insert(temp2D[month[i] - 1].begin(), temp[i]);
    }
    // Loop through all the month vector
    for (int i = 0; i < 12; i++)
    {
        cout << MONTH_LIST[i] << "\t\t";
        // Trigger the serial function to display
        serial_Calculate(temp2D[i]);
    }

    // End Counting
    endTime = clock();

    cout << std::endl;
    cout << "TOTAL COMPLETION TIME: \t" << (endTime - startTime) << " ms" << std::endl
         << std::endl
         << std::endl;
    ;
}

#endif // SERIAL_OPERATION_H