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
void serial_Calculate(vector<float> &values, bool displayQuartile = false, SORT_ALGORITHM sortAlgorithm = SORT_ALGORITHM::SELECTION)
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
    if (sortAlgorithm == MERGE)
        sStats.mergeSort(temperature, SORT_ORDER::ASCENDING); // Perform merge sort - Fastest
    else if (sortAlgorithm == SELECTION)
        sStats.selectionSort(temperature, SORT_ORDER::ASCENDING); // Perform selection sort
    else if (sortAlgorithm == BUBBLE)
        sStats.bubbleSort(temperature, SORT_ORDER::ASCENDING); // Perform bubble sort - Slowest

    // Calculate the variables
    int size = temperature.size();
    float sum = sStats.getSum(temperature);

    float min = temperature[0];
    float max = temperature[size - 1];
    float mean = sum / (size);
    float sDeviation = sStats.getSDeviation(temperature);
    float median = sStats.getMedian(temperature);
    float q1 = 0, q3 = 0;
    if (displayQuartile)
    {
        q1 = sStats.getQ1(temperature);
        q3 = sStats.getQ3(temperature);
    }

    // End counting
    clock_t endTime = clock();

    // Display Data
    displayInfo_Summary(size, mean, sDeviation, min, max, median, q1, q3, startTime, endTime);
}

// Summary Functions
void serial_Overall(vector<float> &temp, SORT_ALGORITHM sortAlgorithm)
{
    // Display overall header
    displayInfo_Overall_Header();

    // Start Counting
    clock_t overallStartTime = clock();

    // Calculate and display the temperature data
    cout << "| " << left << setfill(' ') << setw(14) << "OVERALL";
    serial_Calculate(temp, true, sortAlgorithm);

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_TableDiv('-');
    displayInfo_Footer(overallStartTime, overallEndTime);

    return;
}

void serial_By_Year(vector<float> &temp, vector<int> &year, SORT_ALGORITHM sortAlgorithm)
{
    // Display by year header
    displayInfo_By_Year_Header();

    // Start Counting
    clock_t overallStartTime = clock();

    // Group temperatures by year
    map<int, vector<float>> tempData;
    for (size_t i = 0; i < temp.size(); i++)
        tempData[year[i]].push_back(temp[i]);

    // Calculate and display statistics for each year
    for (const auto &entry : tempData)
    {
        int currentYear = entry.first;
        vector<float> temperatures = entry.second;

        cout << "| " << left << setfill(' ') << setw(14) << currentYear;
        serial_Calculate(temperatures, true, sortAlgorithm); // Calculate and display the temperature data
    }
    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_TableDiv('-');
    displayInfo_Footer(overallStartTime, overallEndTime);

    return;
}

void serial_By_Month(vector<float> &temp, vector<int> &month, SORT_ALGORITHM sortAlgorithm)
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
        serial_Calculate(tempVar[i], false, sortAlgorithm); // Calculate and display the temperature data
    }

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_TableDiv('-');
    displayInfo_Footer(overallStartTime, overallEndTime);

    return;
}

void serial_By_Station(vector<float> &temp, vector<string> &stationName, SORT_ALGORITHM sortAlgorithm)
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
            serial_Calculate(tempVar, true, sortAlgorithm);
            tempVar.clear();

            // Update the current station and add the new temperature
            currentStation = stationName[i];
            tempVar.push_back(temp[i]);
        }
    }

    // Process the last station's data
    cout << "| " << left << setfill(' ') << setw(14) << currentStation;
    serial_Calculate(tempVar, true, sortAlgorithm);

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_TableDiv('-');
    displayInfo_Footer(overallStartTime, overallEndTime);

    return;
}

void serial_By_Year_All_Station(vector<float> &temp, vector<int> &year, vector<string> &stationName, SORT_ALGORITHM sortAlgorithm)
{
    // Display By Station Header
    displayInfo_By_Station_Header();

    // Start Counting
    clock_t overallStartTime = clock();

    // Get unique station names
    unordered_set<string> uniqueStation(stationName.begin(), stationName.end());

    // Group temperatures by year and then station names
    map<int, map<string, vector<float>>> tempData;
    for (size_t i = 0; i < temp.size(); i++)
        tempData[year[i]][stationName[i]].push_back(temp[i]);

    // Calculate and display statistics for each year
    for (const auto &entry : tempData)
    {
        int currentYear = entry.first;                         // The year
        map<string, vector<float>> stationData = entry.second; // The temperature data for each station

        displayInfo_TableDiv(' ');
        cout << "| " << left << setfill(' ') << setw(158) << currentYear << "|" << endl; // Display the year
        displayInfo_TableDiv(' ');

        for (const auto &station : uniqueStation) // For each station
        {
            cout << "| " << left << setfill(' ') << setw(14) << station;
            serial_Calculate(stationData[station], true, sortAlgorithm); // Calculate and display the temperature data
        }
        displayInfo_TableDiv('-');
    }
    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);

    return;
}

void serial_By_Month_All_Station(vector<float> &temp, vector<string> &stationName, vector<int> &month, SORT_ALGORITHM sortAlgorithm)
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
                    serial_Calculate(tempData, false, sortAlgorithm);
                    tempData.clear();
                    copiedUniqueStation.erase(currentStation);

                    // Update the current station and add the new temperature
                    currentStation = stationName[indexMonthData[i][j]];
                    tempData.push_back(temp[indexMonthData[i][j]]);
                }
            }

            // Process the last station's data
            cout << "| " << left << setfill(' ') << setw(14) << currentStation;
            serial_Calculate(tempData, false, sortAlgorithm);
            copiedUniqueStation.erase(currentStation);
        }

        for (const auto &station : copiedUniqueStation)
        {
            cout << "| " << left << setfill(' ') << setw(14) << station;
            serial_Calculate(tempData, false, sortAlgorithm);
        }
        displayInfo_TableDiv('-');
    }

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);
}

void serial_By_Station_All_Year(vector<float> &temp, vector<string> &stationName, vector<int> &year, SORT_ALGORITHM sortAlgorithm)
{
    // Display By Year Header
    displayInfo_By_Year_Header();

    // Start Counting
    clock_t overallStartTime = clock();

    // Group temperatures by Station and then year
    map<string, map<int, vector<float>>> tempData;
    for (size_t i = 0; i < temp.size(); i++)
        tempData[stationName[i]][year[i]].push_back(temp[i]);

    // Calculate and display statistics for each year
    for (const auto &entry : tempData)
    {
        string currentStation = entry.first;             // The station name
        map<int, vector<float>> yearData = entry.second; // The temperature data for each year

        displayInfo_TableDiv(' ');
        cout << "| " << left << setfill(' ') << setw(158) << currentStation << "|" << endl; // Display the station name
        displayInfo_TableDiv(' ');

        for (const auto &year : yearData) // For each year
        {
            string yearString = to_string(year.first);
            vector<float> temperatures = year.second;

            cout << "| " << left << setfill(' ') << setw(14) << yearString;
            serial_Calculate(temperatures, true, sortAlgorithm); // Calculate and display the temperature data
        }
        displayInfo_TableDiv('-');
    }
    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);

    return;
}

void serial_By_Station_All_Month(vector<float> &temp, vector<string> &stationName, vector<int> &month, SORT_ALGORITHM sortAlgorithm)
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
                serial_Calculate(tempData[j], false, sortAlgorithm);
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
    for (int j = 0; j < 12; j++)
    {
        cout << "| " << left << setfill(' ') << setw(14) << MONTH_LIST[j];
        serial_Calculate(tempData[j], false, sortAlgorithm);
    }

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_TableDiv('-');
    displayInfo_Footer(overallStartTime, overallEndTime);
}

void serial_Histogram(vector<float> &temperature, string outputFileName, int binSize, SORT_ALGORITHM sortAlgorithm)
{
    // Step 0. Check if the temperature is empty
    if (temperature.size() == 0)
    {
        displayInfo_Histogram_Header(binSize, 0, 0, 0);
        displayInfo_Histogram_Summary(0, 0, 0);
        cout << "| " << left << setw(158) << setfill(' ') << "No Data" << "|" << endl;
        displayInfo_Footer(0, 0);
        // Save 0 to the file
        ofstream outputFile(outputFileName); // Open the file
        outputFile << "0,0,0" << endl;       // Save to file
        outputFile.close();                  // Close the file
        return;
    }
    else if (temperature.size() == 1)
    {
        displayInfo_Histogram_Header(binSize, 0, 0, 0);
        displayInfo_Histogram_Summary(temperature[0], temperature[0], 1);
        cout << "| " << left << setw(158) << setfill(' ') << "Only 1 Record, Insufficient Data" << "|" << endl;
        displayInfo_Footer(0, 0);
        // Save 0 to the file
        ofstream outputFile(outputFileName);                                       // Open the file
        outputFile << temperature[0] << ',' << temperature[0] << ',' << 1 << endl; // Save to file
        outputFile.close();                                                        // Close the file
        return;
    }

    SerialStatistics sStats = SerialStatistics();
    // Step 1. Start Clock
    clock_t startTime = clock();

    // Step 2. Create a copy of sorted temperature and create a vector to store the frequency of each bins
    vector<float> temp = temperature;
    vector<float> upper_Limits; // upper limit for each bins
    vector<int> frequencies;    // store frequency of each bins

    // Step 3. Sort the temperature, get the minimum and maximum
    if (sortAlgorithm == MERGE)
        sStats.mergeSort(temp, SORT_ORDER::ASCENDING);
    else if (sortAlgorithm == SELECTION)
        sStats.selectionSort(temp, SORT_ORDER::ASCENDING);
    else if (sortAlgorithm == BUBBLE)
        sStats.bubbleSort(temp, SORT_ORDER::ASCENDING);

    float minimum = temp[0];
    float maximum = temp[temp.size() - 1];
    float bin = (maximum - minimum) / binSize;

    // Step 4. Create output vector
    vector<int> histogram_vector(binSize); // histogram results

    // Step 5. Display Header
    displayInfo_Histogram_Header(binSize, bin, minimum, maximum);

    // Step 6. Calculate the histogram
    // Clear the vector and max frequency
    int max_freq = 0;
    upper_Limits.clear();
    frequencies.clear();

    // For each temperature, calculate the frequency
    for (int i = 0; i < temperature.size(); i++)
    {
        float compareVal = minimum + bin;
        int idx = 0;
        while (temperature[i] > compareVal)
        {
            compareVal += bin; // check next range
            idx++;
        }
        if (idx == binSize)
        {
            idx--;
        }
        histogram_vector[idx] += 1;
    }

    // Add the first minimum value
    upper_Limits.push_back(minimum);

    // Step 7. Save the result
    ofstream outputFile(outputFileName);  // Open the file
    for (int i = 1; i < binSize + 1; i++) // Display the histogram
    {
        float binStart = minimum + ((i - 1) * bin);
        float binEnd = minimum + (i * bin);
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

#ifdef SERIAL_PLOT
    // Step 10. Call the python script to plot the histogram
    string command = "python histPlot.py " + outputFileName;
    system(command.c_str());
#endif

    return;
}

void serial_Histogram_By_Year(vector<float> &temp, vector<int> &year, int binSize, SORT_ALGORITHM sortAlgorithm)
{
    // Group temperatures by year
    map<int, vector<float>> tempData;
    for (size_t i = 0; i < temp.size(); i++)
        tempData[year[i]].push_back(temp[i]);

    // Calculate and display statistics for each year
    for (const auto &entry : tempData)
    {
        string currentYear = to_string(entry.first);
        vector<float> temperatures = entry.second;

        displayInfo_ByX_Header(currentYear);
        string outputFileName = "Serial_Histogram_In_Year_" + currentYear + "_(Bin_Size_=_" + to_string(binSize) + ").csv";
        serial_Histogram(temperatures, outputFileName, binSize, sortAlgorithm); // Calculate and display the temperature data
    }

    return;
}

void serial_Histogram_By_Month(vector<float> &temperature, vector<int> &month, int binSize, SORT_ALGORITHM sortAlgorithm)
{
    // Create a 2D vector to store the temperature data by month
    vector<vector<float>> tempVar(12);

    // Step 1. Isolate Temperature data by month
    for (int i = 0; i < temperature.size(); i++)
        tempVar[month[i] - 1].push_back(temperature[i]); // Insert the temperature to the specific month. [[month][temperature]]

    // Step 2. Calculate for each available month
    for (int i = 0; i < tempVar.size(); i++)
    {
        displayInfo_ByX_Header(MONTH_LIST[i]);
        string outputFileName = "Serial_Histogram_In_" + MONTH_LIST[i] + "_(Bin_Size_=_" + to_string(binSize) + ").csv";
        serial_Histogram(tempVar[i], outputFileName, binSize, sortAlgorithm);
    }

    return;
}

void serial_Histogram_By_Station(vector<float> &temperature, vector<string> &stationName, int binSize, SORT_ALGORITHM sortAlgorithm)
{
    // Create a 1D vector to store the temperature data by station
    vector<float> tempVar;
    string currentStation = stationName[0]; // Initialize with the first station name

    string outputFileName = "";

    for (int i = 0; i < temperature.size(); i++)
    {
        if (stationName[i] == currentStation)
        {
            tempVar.push_back(temperature[i]);
        }
        else
        {
            // Print and process the current station's data
            displayInfo_ByX_Header(currentStation);
            outputFileName = "Serial_Histogram_For_" + currentStation + "_(Bin_Size_=_" + to_string(binSize) + ").csv";
            serial_Histogram(tempVar, outputFileName, binSize, sortAlgorithm);
            tempVar.clear();

            // Update the current station and add the new temperature
            currentStation = stationName[i];
            tempVar.push_back(temperature[i]);
        }
    }

    // Process the last station's data
    displayInfo_ByX_Header(currentStation);
    outputFileName = "Serial_Histogram_For_" + currentStation + "_(Bin_Size_=_" + to_string(binSize) + ").csv";
    serial_Histogram(tempVar, outputFileName, binSize, sortAlgorithm);

    return;
}

void serial_Histogram_By_Year_All_Station(vector<float> &temp, vector<int> &year, vector<string> &stationName, int binSize, SORT_ALGORITHM sortAlgorithm)
{
    // Get unique station names
    unordered_set<string> uniqueStation(stationName.begin(), stationName.end());

    // Group temperatures by year and then station names
    map<int, map<string, vector<float>>> tempData;
    for (size_t i = 0; i < temp.size(); i++)
        tempData[year[i]][stationName[i]].push_back(temp[i]);

    // Calculate and display statistics for each station
    for (const auto &entry : tempData)
    {
        int currentYear = entry.first;                         // The year
        map<string, vector<float>> stationData = entry.second; // The temperature data for each station

        for (const auto &station : uniqueStation) // For each station
        {
            displayInfo_ByX_Header(to_string(currentYear), station);
            string outputFileName = "Serial_Histogram_In_Year_" + to_string(currentYear) + "_For_" + station + "_(Bin_Size_=_" + to_string(binSize) + ").csv";
            serial_Histogram(stationData[station], outputFileName, binSize, sortAlgorithm); // Calculate and display the temperature data
        }
    }
    return;
}

void serial_Histogram_By_Month_All_Station(vector<float> &temp, vector<string> &stationName, vector<int> &month, int binSize, SORT_ALGORITHM sortAlgorithm)
{
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
        println();
        string outputFileName = "Serial_Histogram_In_" + MONTH_LIST[i] + "_For_";
        println();

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
                    displayInfo_ByX_Header(MONTH_LIST[i], currentStation);
                    serial_Histogram(tempData, outputFileName + currentStation + "_(Bin_Size_=_" + to_string(binSize) + ").csv", binSize, sortAlgorithm);
                    tempData.clear();
                    copiedUniqueStation.erase(currentStation);

                    // Update the current station and add the new temperature
                    currentStation = stationName[indexMonthData[i][j]];
                    tempData.push_back(temp[indexMonthData[i][j]]);
                }
            }
            // Process the last station's data
            displayInfo_ByX_Header(MONTH_LIST[i], currentStation);
            serial_Histogram(tempData, outputFileName + currentStation + "_(Bin_Size_=_" + to_string(binSize) + ").csv", binSize, sortAlgorithm);
            copiedUniqueStation.erase(currentStation);
        }
        // Process the last station's data
        for (const auto &station : copiedUniqueStation)
        {
            displayInfo_ByX_Header(MONTH_LIST[i], station);
            serial_Histogram(tempData, outputFileName + "_(Bin_Size_=_" + to_string(binSize) + ").csv", binSize, sortAlgorithm);
        }
    }
    return;
}

void serial_Histogram_By_Station_All_Year(vector<float> &temp, vector<int> &year, vector<string> &stationName, int binSize, SORT_ALGORITHM sortAlgorithm)
{
    // Group temperatures by Station and then year
    map<string, map<int, vector<float>>> tempData;
    for (size_t i = 0; i < temp.size(); i++)
        tempData[stationName[i]][year[i]].push_back(temp[i]);

    // Calculate and display statistics for each year
    for (const auto &entry : tempData)
    {
        string currentStation = entry.first;             // The station name
        map<int, vector<float>> yearData = entry.second; // The temperature data for each year

        for (const auto &year : yearData) // For each year
        {
            vector<float> temperatures = year.second; // The temperature data

            displayInfo_ByX_Header(currentStation, to_string(year.first));
            string outputFileName = "Serial_Histogram_For_" + currentStation + "_In_Year_" + to_string(year.first) + "_(Bin_Size_=_" + to_string(binSize) + ").csv";
            serial_Histogram(temperatures, outputFileName, binSize, sortAlgorithm); // Calculate and display the temperature data
        }
    }

    return;
}

void serial_Histogram_By_Station_All_Month(vector<float> &temp, vector<string> &stationName, vector<int> &month, int binSize, SORT_ALGORITHM sortAlgorithm)
{
    // Step 1. Collect data for each station
    vector<vector<float>> tempData(12);     // For each station, store the temperature data for each month
    string currentStation = stationName[0]; // Initialize with the first station name

    // Each station
    string outputFileName = "";
    for (int i = 0; i < temp.size(); i++)
    {
        if (stationName[i] == currentStation)
        {
            tempData[month[i] - 1].push_back(temp[i]);
        }
        else
        {
            // Print and process the current station's data
            outputFileName = "Serial_Histogram_By_" + currentStation;
            for (int j = 0; j < 12; j++)
            {
                displayInfo_ByX_Header(currentStation, MONTH_LIST[j]);
                serial_Histogram(tempData[j], outputFileName + "_In_" + MONTH_LIST[j] + "_(Bin_Size_=_" + to_string(binSize) + ").csv", binSize, sortAlgorithm);
            }
            tempData.clear();
            tempData.resize(12);

            // Update the current station and add the new temperature
            currentStation = stationName[i];
            tempData[month[i] - 1].push_back(temp[i]);
        }
    }
    // Process the last station's data
    for (int j = 0; j < 12; j++)
    {
        displayInfo_ByX_Header(currentStation, MONTH_LIST[j]);
        serial_Histogram(tempData[j], outputFileName + "_In_" + MONTH_LIST[j] + "_(Bin_Size_=_" + to_string(binSize) + ").csv", binSize, sortAlgorithm);
    }
}

#endif // SERIAL_OPERATION_H