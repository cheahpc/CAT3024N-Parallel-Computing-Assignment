#ifndef PARA_OPERATION_H
#define PARA_OPERATION_H

// Includes
#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <map>
#include <unordered_set>
// Custom Includes
#include "ParallelStatistics.h"
#include "Display.h"
#include "Global.h"

using namespace std;

void parallel_Calculate(vector<float> &values, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
{
    // Create an instance of the ParallelStatistics
    ParallelStatistics pStats = ParallelStatistics();

    // Check if the values vector is empty
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

    // Start counting
    clock_t startTime = clock();

    vector<float> temperature = values; // Copy the values to vector

    // Sorting
    // pStats.mergeSort(temperature, context, queue, program, prof_event, ASCENDING); // Perform merge sort
    pStats.selectionSort(temperature, context, queue, program, prof_event, ASCENDING); // Perform selection sort
    // pStats.bubbleSort(temperature, context, queue, program, prof_event, ASCENDING); // Perform bubble sort TODO

    // Get the size of the vector
    int size = temperature.size();
    float sum = pStats.getSum(temperature, context, queue, program, prof_event);

    float min = temperature[0];
    float max = temperature[size - 1];
    float mean = sum / (size);
    float sDeviation = pStats.getSDeviation(temperature, mean, context, queue, program, prof_event);
    float median = pStats.getMedian(temperature);
    float q1 = pStats.getQ1(temperature);
    float q3 = pStats.getQ3(temperature);

    // End counting
    clock_t endTime = clock();

    // Display Data
    displayInfo_Summary(size, mean, sDeviation, min, max, median, q1, q3, startTime, endTime);
}

void parallel_Overall(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
{
    // Display overall header
    displayInfo_Overall_Header();

    // Start Counting
    clock_t overallStartTime = clock();

    // Calculate and display the temperature data
    cout << "| " << left << setfill(' ') << setw(14) << "OVERALL";
    parallel_Calculate(temp, context, queue, program, prof_event);

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);

    return;
}

void parallel_By_Month(vector<float> &temp, vector<int> &month, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
{
    // Display by month header
    displayInfo_By_Month_Header();

    // Start Counting
    clock_t overallStartTime = clock();

    // Create a 1D vector to store the temperature data by month
    vector<vector<float>> tempVar(12);

    // Step 1. Collect data for each month
    for (int i = 0; i < temp.size(); i++)
        tempVar[month[i] - 1].push_back(temp[i]); // Insert the temperature to the specific month. [[month][temperature]]

    // Step 2. Calculate for each available month
    for (int i = 0; i < tempVar.size(); i++)
    {
        cout << "| " << left << setfill(' ') << setw(14) << MONTH_LIST[i];
        parallel_Calculate(tempVar[i], context, queue, program, prof_event); // Calculate and display the temperature data
    }

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);

    return;
}

void parallel_By_Station(vector<float> &temp, vector<string> &stationName, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
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
            parallel_Calculate(tempVar, context, queue, program, prof_event);
            tempVar.clear();

            // Update the current station and add the new temperature
            currentStation = stationName[i];
            tempVar.push_back(temp[i]);
        }
    }

    // Process the last station's data
    cout << "| " << left << setfill(' ') << setw(14) << currentStation;
    parallel_Calculate(tempVar, context, queue, program, prof_event);

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);

    return;
}

void parallel_By_Month_All_Station(vector<float> &temp, vector<string> &stationName, vector<int> &month, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
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
                    parallel_Calculate(tempData, context, queue, program, prof_event);
                    tempData.clear();
                    copiedUniqueStation.erase(currentStation);

                    // Update the current station and add the new temperature
                    currentStation = stationName[indexMonthData[i][j]];
                    tempData.push_back(temp[indexMonthData[i][j]]);
                }
            }

            // Process the last station's data
            cout << "| " << left << setfill(' ') << setw(14) << currentStation;
            parallel_Calculate(tempData, context, queue, program, prof_event);
            copiedUniqueStation.erase(currentStation);
        }

        for (const auto &station : copiedUniqueStation)
        {
            cout << "| " << left << setfill(' ') << setw(14) << station;
            parallel_Calculate(tempData, context, queue, program, prof_event);
        }
        if (i != 11)
            displayInfo_TableDiv('-');
    }

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);
}

void parallel_By_Station_All_Month(vector<float> &temp, vector<string> &stationName, vector<int> &month, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
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
                parallel_Calculate(tempData[j], context, queue, program, prof_event);
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
        parallel_Calculate(tempData[j], context, queue, program, prof_event);
    }

    // End Counting
    clock_t overallEndTime = clock();

    // Display the footer
    displayInfo_Footer(overallStartTime, overallEndTime);
}

void parallel_Histogram(vector<float> &temperature, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
{
    ParallelStatistics pStats = ParallelStatistics();

    // Step 1. Start Clock
    clock_t startTime = clock();

    // Step 2. Create copy of sorted temperature and create a vector to store the frequency of each bins
    vector<float> temp = temperature;
    vector<float> upper_Limits; // upper limit for each bins
    vector<int> frequencies;    // store frequency of each bins

    // Step 3. Sort the temperature
    pStats.mergeSort(temp, context, queue, program, prof_event, ASCENDING); // Perform merge sort
    // pStats.selectionSort(temp, context, queue, program, prof_event, ASCENDING); // Perform selection sort
    // pStats.bubbleSort(temp, context, queue, program, prof_event, ASCENDING); // Perform bubble sort
    float minimum = temp[0];
    float maximum = temp[temp.size() - 1];
    float binSize = (maximum - minimum) / HISTOGRAM_BIN_NO;

    // Step 4. Create a kernel
    cl::Kernel kernel(program, "p_Hist");
    size_t local_size = 256;

    // Step 5. Adjust padding
    size_t padding_size = temperature.size() % local_size; // 512 / 1024 / 2048 / 4096
    if (padding_size)
    {
        vector<int> temperature_ext(local_size - padding_size, 1000);                          // create an extra vector with neutral values
        temperature.insert(temperature.end(), temperature_ext.begin(), temperature_ext.end()); // append that extra vector to our input
    }

    size_t vector_elements = temperature.size();           // number of elements
    size_t vector_size = temperature.size() * sizeof(int); // size in bytes for local memory

    // Step 6. Create vectors to store the histogram results
    vector<int> histogram_vector(HISTOGRAM_BIN_NO); // histogram results
    vector<int> output(histogram_vector.size());

    // Step 7. Create buffers
    size_t output_size = output.size() * sizeof(float);
    cl::Buffer input_buffer(context, CL_MEM_READ_WRITE, vector_size);
    cl::Buffer output_buffer(context, CL_MEM_READ_WRITE, output_size);

    // Step 8. Copy the input data to the device
    queue.enqueueWriteBuffer(input_buffer, CL_TRUE, 0, vector_size, &temperature[0]);
    queue.enqueueFillBuffer(output_buffer, 0, 0, output_size); // fill the output buffer with zeros for the histogram

    // Step 9. Set kernel arguments
    kernel.setArg(0, input_buffer);
    kernel.setArg(1, output_buffer);
    kernel.setArg(2, HISTOGRAM_BIN_NO);
    kernel.setArg(3, minimum);
    kernel.setArg(4, maximum);
    kernel.setArg(5, binSize);

    // Step 10. Run the kernel
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(vector_elements), cl::NDRange(local_size));

    // Step 11. Read the output data and get the histogram execution time
    queue.enqueueReadBuffer(output_buffer, CL_TRUE, 0, output_size, &histogram_vector[0]);
    unsigned long hist_Exe_Time = prof_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event.getProfilingInfo<CL_PROFILING_COMMAND_START>();

    // Step 12. Display Header
    displayInfo_Histogram_Header(HISTOGRAM_BIN_NO, binSize, minimum, maximum, hist_Exe_Time);

    // Step 13. Clear the vector and max frequency
    int max_freq = 0;
    upper_Limits.clear();
    frequencies.clear();

    // Step 14. Display the histogram summary, save to file
    upper_Limits.push_back(minimum);
    ofstream outputFile(PARALLEL_HISTOGRAM_CSV); // Open the file
    for (int i = 1; i < HISTOGRAM_BIN_NO + 1; i++)
    {
        float binStart = minimum + (i - 1) * binSize;
        float binEnd = minimum + i * binSize;
        int frequency = (histogram_vector[i - 1]);

        displayInfo_Histogram_Summary(binStart, binEnd, frequency);          // Print histogram summary
        outputFile << binStart << ',' << binEnd << ',' << frequency << endl; // Save to file

        max_freq = (frequency > max_freq) ? frequency : max_freq; // Get the maximum frequency
        frequencies.push_back(frequency);
        upper_Limits.push_back(binEnd);
    }
    outputFile.close();
    frequencies.push_back(max_freq);

    // Step 15. End Clock
    clock_t endTime = clock();

    // Step 16. Display Footer
    displayInfo_Footer(startTime, endTime);
}

#endif // PARA_OPERATION_H