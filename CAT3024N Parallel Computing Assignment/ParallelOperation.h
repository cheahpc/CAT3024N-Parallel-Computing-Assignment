#ifndef PARA_OPERATION_H
#define PARA_OPERATION_H

// Includes

#include <vector>
#include <iostream>
#include <ctime>
#include <algorithm>
// Custom Includes
#include "ParallelStatistics.h"

using namespace std;

void Parallel(vector<float> &Values, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
{
    // Start counting
    float startTime = clock();
    std::vector<float> temperature = Values;

    // Get the size of the vector
    int Size = temperature.size();
    // Sort the vector into ascending
    Sort(temperature, context, queue, program, prof_event);
    // Calculate the Sum
    float Sum = SumVec(temperature, context, queue, program, prof_event);
    // Calculate the mean
    float Mean = Sum / (Size);
    // Calculate the standard deviation
    float sDeviation = STDVec(temperature, Mean, context, queue, program, prof_event);
    // Calculate the Min
    float MIN = temperature[0];
    // Calculate the Max
    float MAX = temperature[Size - 1];
    // Calculate the Median
    float Median = SStats.getMedian(temperature);
    // Calculate the 1st Quartile
    float Q1 = SStats.getQ1(temperature);
    // Calculate the 3rd Quartile
    float Q3 = SStats.getQ3(temperature);

    // End Counting
    float endTime = clock();

    // Display Data
    displayInfo_Result(Size, Sum, Mean, sDeviation, MIN, MAX, Median, Q1, Q3, parallel_displayOverall, startTime, endTime);
}

void Parallel_Summary(bool displayOverall = false, vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, vector<string> &stationName, vector<int> &month)
{
    displayOverall = false; // Set global parallel display type

    // Start Counting for Station
    float startTime = clock();

    // Display results for stations
    std::cout << "==================================[STATION RESULT]==================================" << std::endl;
    std::cout << "STATION    \tMIN \tMAX \tMEAN \tSD \tMEDIAN \t1Q \t3Q \tSIZE \tTIME" << std::endl;
    std::cout << "====================================================================================" << std::endl;

// Part of temperatures belong to a specific station
std:
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
                    std::cout << stationName[i] << "  \t";
                    Parallel(partTemp, context, queue, program, prof_event);
                    partTemp.clear(); // Reset the partTemp
                }
            }
            else
            {
                // Last temperature data
                partTemp.insert(partTemp.begin(), temp[i]);
                std::cout << stationName[i] << " \t\t";
                Parallel(partTemp, context, queue, program, prof_event);
                partTemp.clear(); // Reset the partTemp
            }
        }
    }

    // End Counting for Station
    float endTime = clock();

    std::cout << std::endl;
    std::cout << "TOTAL COMPLETION TIME: \t" << (endTime - startTime) << " ms" << std::endl
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
        std::cout << monthsList[i] << "\t\t";
        // Trigger the paralllel function to display
        Parallel(temp2D[i], context, queue, program, prof_event);
    }

    // End Counting
    endTime = clock();

    std::cout << std::endl;
    std::cout << "TOTAL COMPLETION TIME: \t" << (endTime - startTime) << " ms" << std::endl
              << std::endl
              << std::endl;
    ;

    parallel_displayOverall = true; // Set global parallel display type
}

void Histogram_Parallel(std::vector<float> &temperature, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, float minimum, float maximum)
{
    cl::Kernel kernel(program, "hist_simple");

    // the following part adjusts the length of the input vector so it can be run for a specific workgroup size
    // if the total input length is divisible by the workgroup size
    // this makes the code more efficient
    size_t local_size = 256; // 1024; //work group size - higher work group size can reduce
    size_t padding_size = temperature.size() % local_size;

    // if the input vector is not a multiple of the local_size
    // insert additional neutral elements (0 for addition) so that the total will not be affected (make work for my working set of data)
    if (padding_size)
    {
        // create an extra vector with neutral values
        std::vector<int> temperature_ext(local_size - padding_size, 1000);
        // append that extra vector to our input
        temperature.insert(temperature.end(), temperature_ext.begin(), temperature_ext.end());
    }

    size_t vector_elements = temperature.size();           // number of elements
    size_t vector_size = temperature.size() * sizeof(int); // size in bytes

    // Create output vector
    vector<int> histogram_vector(histogram_bin_no); // histogram results
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
    kernel.setArg(2, histogram_bin_no);
    kernel.setArg(3, minimum);
    kernel.setArg(4, maximum);

    // Execute kernel
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(vector_elements), cl::NDRange(local_size));

    // Copy the result from device to host
    queue.enqueueReadBuffer(output_buffer, CL_TRUE, 0, output_size, &histogram_vector[0]);

    unsigned long HistogramExecution = prof_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event.getProfilingInfo<CL_PROFILING_COMMAND_START>();

    // display bins and frequency
    std::cout << "Minimum: " << minimum << ", Maximum: " << maximum << std::endl;
    std::cout << "Number of Bins: " << histogram_bin_no << ", Bin Size: " << (maximum - minimum) / histogram_bin_no << std::endl;
    float binSize = (maximum - minimum) / histogram_bin_no;
    std::printf("%s %d ns\n", "Histogram Execution Time:", HistogramExecution);
    int max_freq = 0;

    // clear vectors
    upperLimits.clear();
    frequencies.clear();

    // first element is the minimum of elements
    upperLimits.push_back(minimum);

    for (int i = 1; i < histogram_bin_no + 1; i++)
    {
        float binStart = minimum + (i - 1) * binSize;
        float binEnd = minimum + i * binSize;
        int frequency = (histogram_vector[i - 1]);
        std::cout << "Bin Range: >" << binStart << " to <=" << binEnd << ", Frequency: " << frequency << std::endl;

        max_freq = (frequency > max_freq) ? frequency : max_freq;
        frequencies.push_back(frequency);
        upperLimits.push_back(binEnd);
    }

    // last element is the total number of frequencies
    frequencies.push_back(max_freq);
}

#endif // PARA_OPERATION_H