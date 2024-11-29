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
    if (values.size() == 0)
    {
        println("Error: The size of the vector is zero. Please check the input vector and try again...");
        pause();
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

    float mean = sum / (size);
    float sDeviation = pStats.getSDeviation(temperature, mean, context, queue, program, prof_event);
    float min = temperature[0];
    float max = temperature[size - 1];
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

// TODO Split summary into sub functions
void Parallel_Summary(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, vector<string> &stationName, vector<int> &month)
{
    // Start Counting for Station
    clock_t startTime = clock();

    // Display results for stations
    // TODO Display Header

    // Part of temperatures belong to a specific station
    vector<float> partTemp;
    for (int i = 0; i < temp.size(); i++)
    {
        if (partTemp.size() == 0)
        {
            partTemp.insert(partTemp.begin(), temp[i]);
        }
        else
        {
            // Check if the next station is matched with the current station
            if ((i + 1) != temp.size())
            { // if not last data
                // if matched, continue adding the temparature to partTemp
                if (stationName[i] == stationName[i + 1])
                {
                    partTemp.insert(partTemp.begin(), temp[i]);
                }
                else
                {
                    partTemp.insert(partTemp.begin(), temp[i]);
                    cout << stationName[i] << "  \t";
                    parallel_Calculate(partTemp, context, queue, program, prof_event);
                    partTemp.clear(); // Reset the partTemp
                }
            }
            else
            {
                // Last temperature data
                partTemp.insert(partTemp.begin(), temp[i]);
                std::cout << stationName[i] << " \t\t";
                parallel_Calculate(partTemp, context, queue, program, prof_event);
                partTemp.clear(); // Reset the partTemp
            }
        }
    }

    // End Counting for Station
    clock_t endTime = clock();

    cout << std::endl;
    cout << "TOTAL COMPLETION TIME: \t" << (endTime - startTime) << " ms" << std::endl
         << std::endl
         << std::endl;
    ;

    // Start Counting Months
    startTime = clock();

    // Display the results for 12 months
    cout << "==================================[MONTHS RESULT]===================================" << std::endl;
    cout << "MONTH    \tMIN \tMAX \tMEAN \tSD \tMEDIAN \t1Q \t3Q \tSIZE \tTIME" << std::endl;
    cout << "====================================================================================" << std::endl;

    // Part of temperature belong to a specific month
    std::vector<std::vector<float>> temp2D(12);

    // Loop through all the temperature data
    for (int i = 0; i < temp.size(); i++)
    {
        temp2D[month[i] - 1].insert(temp2D[month[i] - 1].begin(), temp[i]);
    }
    // Loop through all the month vector
    for (int i = 0; i < 12; i++)
    {
        std::cout << MONTH_LIST[i] << "\t\t";
        // Trigger the paralllel function to display
        parallel_Calculate(temp2D[i], context, queue, program, prof_event);
    }

    // End Counting
    endTime = clock();

    cout << endl;
    cout << "TOTAL COMPLETION TIME: \t" << (endTime - startTime) << " ms" << std::endl
         << endl
         << endl;
    ;
}

void Histogram_Parallel(vector<float> &temperature, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, float minimum, float maximum)
{
    cl::Kernel kernel(program, "hist_simple");

    vector<float> upper_Limits; // upper limit for each bins
    vector<int> frequencies;    // store frequency of each bins

    // the following part adjusts the length of the input vector so it can be run for a specific workgroup size
    // if the total input length is divisible by the workgroup size
    // this makes the code more efficient

    size_t padding_size = temperature.size() % LOCAL_SIZE;

    // if the input vector is not a multiple of the local_size
    // insert additional neutral elements (0 for addition) so that the total will not be affected (make work for my working set of data)
    if (padding_size)
    {
        // create an extra vector with neutral values
        vector<int> temperature_ext(LOCAL_SIZE - padding_size, 1000);
        // append that extra vector to our input
        temperature.insert(temperature.end(), temperature_ext.begin(), temperature_ext.end());
    }

    size_t vector_elements = temperature.size();           // number of elements
    size_t vector_size = temperature.size() * sizeof(int); // size in bytes

    // Create output vector
    vector<int> histogram_vector(HISTOGRAM_BIN_NO); // histogram results
    vector<int> output(histogram_vector.size());
    size_t output_size = output.size() * sizeof(float);

    // Create buffers
    cl::Buffer input_buffer(context, CL_MEM_READ_WRITE, vector_size);
    cl::Buffer output_buffer(context, CL_MEM_READ_WRITE, output_size);

    // Create queue and copy vectors to device memory
    queue.enqueueWriteBuffer(input_buffer, CL_TRUE, 0, vector_size, &temperature[0]);
    queue.enqueueFillBuffer(output_buffer, 0, 0, output_size); // zero B buffer on device memory

    // Set the arguments 0 and 3 to be the buffers
    kernel.setArg(0, input_buffer);
    kernel.setArg(1, output_buffer);
    kernel.setArg(2, HISTOGRAM_BIN_NO);
    kernel.setArg(3, minimum);
    kernel.setArg(4, maximum);

    // Execute kernel
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(vector_elements), cl::NDRange(LOCAL_SIZE));

    // Copy the result from device to host
    queue.enqueueReadBuffer(output_buffer, CL_TRUE, 0, output_size, &histogram_vector[0]);

    unsigned long HistogramExecution = prof_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event.getProfilingInfo<CL_PROFILING_COMMAND_START>();

    // display bins and frequency
    cout << "Minimum: " << minimum << ", Maximum: " << maximum << std::endl;
    cout << "Number of Bins: " << HISTOGRAM_BIN_NO << ", Bin Size: " << (maximum - minimum) / HISTOGRAM_BIN_NO << std::endl;
    float binSize = (maximum - minimum) / HISTOGRAM_BIN_NO;
    printf("%s %d ns\n", "Histogram Execution Time:", HistogramExecution);
    int max_freq = 0;

    // clear vectors
    upper_Limits.clear();
    frequencies.clear();

    // first element is the minimum of elements
    upper_Limits.push_back(minimum);

    for (int i = 1; i < HISTOGRAM_BIN_NO + 1; i++)
    {
        float binStart = minimum + (i - 1) * binSize;
        float binEnd = minimum + i * binSize;
        int frequency = (histogram_vector[i - 1]);
        std::cout << "Bin Range: >" << binStart << " to <=" << binEnd << ", Frequency: " << frequency << std::endl;

        max_freq = (frequency > max_freq) ? frequency : max_freq;
        frequencies.push_back(frequency);
        upper_Limits.push_back(binEnd);
    }

    // last element is the total number of frequencies
    frequencies.push_back(max_freq);
}

#endif // PARA_OPERATION_H