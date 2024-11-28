#include "ParallelStatistics.h"

using namespace std;

// Empty Constructor
ParallelStatistics::ParallelStatistics()
{
}

// Empty Destructor
ParallelStatistics::~ParallelStatistics()
{
}

// TODO Refactor Kernel Execs
// Kernel Execution function
void ParallelStatistics::KernelExec(cl::Kernel kernel, vector<float> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event &prof_event, string Name)
{
    // Get the size of the vector
    double Size = temp.size();

    // Get the number of input elements
    size_t input_elements = temp.size();
    // Size in bytes of the input vector
    size_t input_size = temp.size() * sizeof(float);

    // Define Output vector B
    vector<float> B(input_elements);
    // Get the size in bytes of the output vector
    size_t output_size = B.size() * sizeof(float);

    // Setup device buffer
    cl::Buffer buffer_A(context, CL_MEM_READ_ONLY, input_size);
    cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, output_size);

    // Write all the values from temp into buffer
    queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, input_size, &temp[0], NULL, &prof_event);
    queue.enqueueFillBuffer(buffer_B, 0, 0, output_size);

    // Set the arguments 0 and 1 to be the buffers
    kernel.setArg(0, buffer_A); // Buffer A is the input vector
    kernel.setArg(1, buffer_B); // Buffer B is the output vector

    // If two is true then set argument two to the local memory size, providing each workgroup with local memory
    if (Two == true)
        kernel.setArg(2, cl::Local(Local_Size * sizeof(float))); // Local memory size
    // If three is true then set argument three to the float value passed into the function
    if (Three == true)
        kernel.setArg(3, FThree);
    // If four is true then set argument three to the int value passed into the function
    if (Four == true)
        kernel.setArg(4, IFour);

    // Run the kernel
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(Local_Size), NULL, &prof_event);

    // Copy the result from device to host
    // Setup prof Event
    cl::Event prof_event2;
    queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, output_size, &temp[0], NULL, &prof_event2); // Copy the result from device to host
}

float ParallelStatistics::KernelExecRet(cl::Kernel kernel, vector<float> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event &prof_event, string Name)
{
    // Get the size of the vector
    double Size = temp.size();

    // Get the number of input elements
    size_t input_elements = temp.size();
    // Size in bytes of the input vector
    size_t input_size = temp.size() * sizeof(float);

    // Define Output vector B
    std::vector<float> B(input_elements);
    // Get the size in bytes of the output vector
    size_t output_size = B.size() * sizeof(float);

    // Setup device buffer
    cl::Buffer buffer_A(context, CL_MEM_READ_ONLY, input_size);
    cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, output_size);

    // Set the arguments 0 and 1 to be the buffers
    queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, input_size, &temp[0], NULL, &prof_event);

    // Display kernel memory write time
    // std::cout << Name << " Kernel memory write time [ns]:" << prof_event.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event.getProfilingInfo<CL_PROFILING_COMMAND_START>() << std::endl;
    // std::cout << GetFullProfilingInfo(prof_event, ProfilingResolution::PROF_US) << std::endl << std::endl;

    queue.enqueueFillBuffer(buffer_B, 0, 0, output_size); // zero B buffer on device memory

    // Set the arguments 0 and 1 to be the buffers
    kernel.setArg(0, buffer_A);
    kernel.setArg(1, buffer_B);

    // If two is true then set argument two to the local memory size
    if (Two == true)
        kernel.setArg(2, cl::Local(Local_Size * sizeof(float))); // Local memory size
    // If three is true then set argument three to the float value passed into the function
    if (Three == true)
        kernel.setArg(3, FThree);
    // If four is true then set argument three to the int value passed into the function
    if (Four == true)
        kernel.setArg(4, IFour);

    // Run the kernel [Kernel, Offset, GlobalSize, LocalSize, Event]
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(Local_Size), NULL, &prof_event);

    // Copy the result from device to host
    // Setup prof Event
    cl::Event prof_event2;
    // Copy the result from device to host [Buffer, Blocking, Offset, Size, HostPtr, Event]
    queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, output_size, &B[0], NULL, &prof_event2);

    // Display kernel memory read time
    // std::cout << Name << " Kernel memory read time [ns]:" << prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_START>() << std::endl;
    // std::cout << GetFullProfilingInfo(prof_event2, ProfilingResolution::PROF_US) << std::endl << std::endl;

    // Return the first element of the buffer Vector B
    return B[0];
}

// Add Padding function
int ParallelStatistics::AddPadding(vector<float> &temp, size_t LocalSize, float PadVal)
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
    int padding_size = AddPadding(temp, LOCAL_SIZE, -1000000.0f); // Add padding to the vector

    cl::Kernel kernel = cl::Kernel(program, "p_SelectionSort"); // Set kernel to the parallel selection kernel

    KernelExec(kernel, temp, LOCAL_SIZE, context, queue, true, false, false, 0.0f, 0, prof_event, "Parallel Selection Sort"); // Perform the kernel

    temp.erase(temp.begin(), temp.begin() + (LOCAL_SIZE - padding_size)); // Erase the padded elements at the start of the vector
}

void ParallelStatistics::mergeSort(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, SORT_ORDER mode)
{
    // Add padding to the vector
    int padding_size = AddPadding(temp, LOCAL_SIZE, 0.0f);

    cl::Kernel kernel = cl::Kernel(program, "p_MergeSort"); // Set kernel to the parallel merge sort kernel

    KernelExec(kernel, temp, LOCAL_SIZE, context, queue, true, false, false, 0.0f, 0, prof_event, "Parallel Merge Sort"); // Perform the kernel

    temp.erase(temp.begin(), temp.begin() + (LOCAL_SIZE - padding_size)); // Erase the padded elements at the start of the vector
}

// Statistics Functions
float ParallelStatistics::getSum(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
{

    // Add padding to the vector
    int padding_size = AddPadding(temp, LOCAL_SIZE, 0.0f);
    // Set kernel to the reduce addition kernel
    cl::Kernel kernel = cl::Kernel(program, "reduce_Sum");
    // Set return to the output from kernel execution
    float Return = KernelExecRet(kernel, temp, LOCAL_SIZE, context, queue, true, false, false, 0.0f, 0, prof_event, "Sum Vector");
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
    // Get the size of the input vector
    int Size = values.size();
    // Variable to hold middle value
    float Middle;

    // Check if size is an even number
    if (Size % 2 == 0)
    {
        // Calculate the difference between the two middle values
        float Difference = values[(Size / 2) - 1] - values[(Size / 2)];
        // Account for difference to find the true median value
        Middle = values[(Size / 2) - 1] - Difference / 2;
    }
    else
    {
        Middle = values[(Size / 2)]; // Get center element
    }

    return Middle;
}

float ParallelStatistics::getSDeviation(vector<float> &temp, float Mean, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
{
    // Get the size of the vector
    double Size = temp.size();
    // Add padding to the vector
    int padding_size = AddPadding(temp, LOCAL_SIZE, 0.0f);
    // Set kernel to the reduce standard deviation kernel
    cl::Kernel kernel = cl::Kernel(program, "reduce_STD_4");
    // Set return to the output from kernel execution
    float Return = KernelExecRet(kernel, temp, LOCAL_SIZE, context, queue, true, true, true, Mean, padding_size, prof_event, "Standard Deviation");
    // Divide the result by the size
    Return = (Return / (Size));
    // Square root the result
    Return = sqrt(Return);

    return Return;
}

float ParallelStatistics::getQ1(vector<float> &values)
{
    // Get the size of the input vector
    int Size = values.size();
    // Variable to hold the first quartile value
    float First;
    // Check if size is an even number
    if (Size % 2 == 0)
    {
        // Calculate the difference between the two first quartile values
        float Difference = values[(Size / 4) - 1] - values[(Size / 4)];
        // Account for difference to find the true first quartile value
        First = values[(Size / 4) - 1] - Difference / 2;
    }
    else
    {
        // Get the value of the first quartile
        First = values[(Size / 4)];
    }

    return First;
}

float ParallelStatistics::getQ3(vector<float> &values)
{
    // Get the size of the input vector
    int Size = values.size();
    // Variable to hold the third quartile value
    float Third;
    // Set pos to the position of the third quartile
    int pos = (3 * Size) / 4;
    // Check if size is an even number
    if (Size % 2 == 0)
    {
        // Calculate the difference between the two third quartile values
        float Difference = values[pos - 1] - values[pos];
        // Account for difference to find the true third quartile value
        Third = values[pos - 1] - Difference / 2;
    }
    else
    {
        // Get the value of the third quartile
        Third = values[pos];
    }

    return Third;
}
