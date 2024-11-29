#include "ParallelStatistics.h"

// #define SHOW_PROFILING

using namespace std;

// Empty Constructor
ParallelStatistics::ParallelStatistics()
{
}

// Empty Destructor
ParallelStatistics::~ParallelStatistics()
{
}

// Kernel Execution function
#pragma region Kernel
float ParallelStatistics::kernelExecute(bool isReturn, // Should function return value or write to vector
                                        cl::Kernel kernel, vector<float> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue,
                                        bool arg2, bool arg3, bool arg4,
                                        float float3, int int4, // 2 for Local, 3 for Float (Mean), 4 for Int (padding size),
                                        cl::Event &prof_event, string Name)
{
    // Step 1: Get the number of input elements
    size_t input_elements = temp.size();             // Get the number of input elements
    size_t input_size = temp.size() * sizeof(float); // Size in bytes of the input vector
    vector<float> output_Vect(input_elements);

    size_t output_size = output_Vect.size() * sizeof(float);

    // Step 2: Setup device buffer
    cl::Buffer buffer_A(context, CL_MEM_READ_ONLY, input_size);
    cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, output_size);

    // Step 3: Write all the values from temp into buffer
    queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, input_size, &temp[0], NULL, &prof_event);
// Step 3.1: Display kernel memory write time
#ifdef SHOW_PROFILING
    cout << Name << " Kernel memory write time [ns]:" << prof_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event.getProfilingInfo<CL_PROFILING_COMMAND_START>() << endl;
#endif
    queue.enqueueFillBuffer(buffer_B, 0, 0, output_size);

    // Step 4: Set the arguments, 0 and 1 to be the buffers (Input and Output)
    kernel.setArg(0, buffer_A); // Buffer A is the input vector
    kernel.setArg(1, buffer_B); // Buffer B is the output vector

    if (arg2)
        kernel.setArg(2, cl::Local(Local_Size * sizeof(float))); // Allocate local memory
    if (arg3)
        kernel.setArg(3, float3);
    if (arg4)
        kernel.setArg(4, int4);

    // Step 5: Run the kernel
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(Local_Size), NULL, &prof_event);

    // Step 6: Setup prof Event
    cl::Event prof_event2;

    // Step 7: Output the result from device to host
    if (isReturn)
    {
        queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, output_size, &output_Vect[0], NULL, &prof_event2);
        // Step 7.1: Display kernel memory read time
#ifdef SHOW_PROFILING
        cout << Name << " Kernel memory read time [ns]:" << prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_START>() << endl;
#endif
        return output_Vect[0];
    }
    else
    {
        queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, output_size, &temp[0], NULL, &prof_event2); // Copy the result from device to host
    }
}
#pragma endregion

// Add Padding function
int ParallelStatistics::addPadding(vector<float> &temp, size_t LocalSize, float PadVal)
{
    // Find the padding size
    int padding_size = temp.size() % LOCAL_SIZE;
    // If there is padding size then
    if (padding_size)
    {
        // Create an extra vector with PadVal values
        vector<float> A_ext(LOCAL_SIZE - padding_size, PadVal);
        // Append that extra vector to the input
        temp.insert(temp.end(), A_ext.begin(), A_ext.end());
    }
    // Return padding_size
    return padding_size;
}

// Sort function
void ParallelStatistics::bubbleSort(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, SORT_ORDER mode)
{
    // TODO Implement Parallel Bubble Sort
}

void ParallelStatistics::selectionSort(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, SORT_ORDER mode)
{
    int padding_size = addPadding(temp, LOCAL_SIZE, 0.0f); // Add padding to the vector
    cl::Kernel kernel;
    // Set kernel to the parallel selection kernel
    if (mode == ASCENDING)
        kernel = cl::Kernel(program, "p_SelectionSortAsc"); // Set kernel to the parallel selection kernel
    else if (mode == DESCENDING)
        kernel = cl::Kernel(program, "p_SelectionSortDesc"); // Set kernel to the parallel selection kernel

    kernelExecute(false, kernel, temp, LOCAL_SIZE, context, queue,
                  false, false, false, 0.0f, 0,           // 2 for Local, 3 for Float (Mean), 4 for Int (padding size),
                  prof_event, "Parallel Selection Sort"); // Perform the kernel

    temp.erase(temp.begin(), temp.begin() + (LOCAL_SIZE - padding_size)); // Erase the padded elements at the start of the vector
}

void ParallelStatistics::mergeSort(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, SORT_ORDER mode)
{
    // Add padding to the vector
    int padding_size = addPadding(temp, LOCAL_SIZE, 0.0f); // Padding not needed for merge sort

    // cl::Kernel kernel = cl::Kernel(program, "p_MergeSort"); // Set kernel to the parallel merge sort kernel
    cl::Kernel kernel = cl::Kernel(program, "p_MergeSort"); // Set kernel to the parallel merge sort kernel

    kernelExecute(false, kernel, temp, LOCAL_SIZE, context, queue,
                  true, false, false, 0.0f, 0,       // 2 for Local, 3 for Float (Mean), 4 for Int (padding size)
                  prof_event, "Parallel Merge Sort"); // Perform the kernel

    temp.erase(temp.begin(), temp.begin() + (LOCAL_SIZE - padding_size)); // Erase the padded elements at the start of the vector
}

// Statistics Functions
float ParallelStatistics::getSum(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
{
    // Add padding to the vector
    int padding_size = addPadding(temp, LOCAL_SIZE, 0.0f);
    // Set kernel to the reduce addition kernel
    cl::Kernel kernel = cl::Kernel(program, "p_Sum");
    // Set return to the output from kernel execution
    float Return = kernelExecute(true, kernel, temp, LOCAL_SIZE, context, queue,
                                 true, false, false, 0.0f, 0, // 2 for Local, 3 for Float (Mean), 4 for Int (padding size)
                                 prof_event, "Sum Vector");
    // Return value
    return Return;
}

float ParallelStatistics::getMinMax(vector<float> &values, bool getMin = true)
{
    // TODO Implement Parallel MinMax
    return 0;
}

float ParallelStatistics::getMedian(vector<float> &values)
{
    int Size = values.size(); // Get the size of the input vector
    float Middle;

    // Check if size is an even number
    if (Size % 2 == 0)
    {
        float Difference = values[(Size / 2) - 1] - values[(Size / 2)]; // Calculate the difference between the two middle values
        Middle = values[(Size / 2) - 1] - Difference / 2;               // Account for difference to find the true median value
    }
    else
    {
        Middle = values[(Size / 2)]; // Get center element
    }

    return Middle;
}

float ParallelStatistics::getSDeviation(vector<float> &temp, float Mean, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
{
    // Get the size of the vector, set padding
    int padding_size = addPadding(temp, LOCAL_SIZE, 0.0f);

    cl::Kernel kernel = cl::Kernel(program, "p_Standard_Deviation"); // Set kernel to the reduce standard deviation kernel
    // Set return to the output from kernel execution
    float result = kernelExecute(true, kernel, temp, LOCAL_SIZE, context, queue,
                                 true, true, true, Mean, padding_size, // 2 for Local, 3 for Float (Mean), 4 for Int (padding size), 5 for int (Sort order)
                                 prof_event, "Standard Deviation");
    // Square root the (result / size)
    result = sqrt((result / temp.size()));

    return result;
}

float ParallelStatistics::getQ1(vector<float> &values)
{
    // Get the size of the input vector
    int Size = values.size();
    float First;

    // Check if size is an even number
    if (Size % 2 == 0)
    {
        float Difference = values[(Size / 4) - 1] - values[(Size / 4)];
        First = values[(Size / 4) - 1] - Difference / 2; // Account for difference to find the true first quartile value
    }
    else
    {

        First = values[(Size / 4)]; // Get the value of the first quartile
    }

    return First;
}

float ParallelStatistics::getQ3(vector<float> &values)
{
    // Get the size of the input vector
    int Size = values.size();
    float Third;

    // Set pos to the position of the third quartile
    int pos = (3 * Size) / 4;
    // Check if size is an even number
    if (Size % 2 == 0)
    {
        float Difference = values[pos - 1] - values[pos]; // Calculate the difference between the two third quartile values
        Third = values[pos - 1] - Difference / 2;         // Account for difference to find the true third quartile value
    }
    else
    {
        Third = values[pos]; // Get the value of the third quartile
    }

    return Third;
}
