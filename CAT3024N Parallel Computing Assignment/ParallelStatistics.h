#ifndef PARALLEL_STATISTICS_H
#define PARALLEL_STATISTICS_H

// Includes
#include <CL/openCL.hpp>
#include <iostream>
#include <string>
#include <vector>

// Custom Includes
#include "Global.h"

using namespace std;

class ParallelStatistics
{
private:
    // Kernel Executions
    void ParallelStatistics::KernelExec(cl::Kernel kernel, vector<float> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event &prof_event, string Name);
    float ParallelStatistics::KernelExecRet(cl::Kernel kernel, vector<float> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event &prof_event, string Name);

    // Add Padding
    int AddPadding(vector<float> &temp, size_t LocalSize, float PadVal);

public:
    // Empty Constructor and Destructor
    ParallelStatistics();
    ~ParallelStatistics();

    // Function Declarations
    // Sort Functions
    void ParallelStatistics::bubbleSort(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, SORT_ORDER mode);
    void ParallelStatistics::selectionSort(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, SORT_ORDER mode);
    void ParallelStatistics::mergeSort(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, SORT_ORDER mode);

    void ParallelStatistics::Display(vector<float> &values); // Displays a vector

    // Statistics Functions
    float ParallelStatistics::getSum(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event);
    float ParallelStatistics::getMinMax(vector<float> &values, bool getMin);
    float ParallelStatistics::getMedian(vector<float> &values);
    float ParallelStatistics::getSDeviation(vector<float> &temp, float Mean, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event);
    float ParallelStatistics::getQ1(vector<float> &values);
    float ParallelStatistics::getQ3(vector<float> &values);
};
#endif // PARALLEL_STATISTICS_H