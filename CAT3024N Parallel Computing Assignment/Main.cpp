#define __CL_ENABLE_EXCEPTIONS

// Includes
#include <CL/openCL.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <windows.h>
// Custom Includes
#include "Display.h"
#include "OpenCLUtils.h"
#include "Sort.h"
#include "StationData.h"
#include "SerialOperation.h"

// Function Declaration
float SumVec(std::vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event);
float STDVec(std::vector<float> &temp, float Mean, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event);
void Sort(std::vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event);
int AddPadding(std::vector<float> &temp, size_t LocalSize, float PadVal);
void KernelExec(cl::Kernel kernel, std::vector<float> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event &prof_event, std::string Name);
float KernelExecRet(cl::Kernel kernel, std::vector<float> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event &prof_event, std::string Name);

vector<float> updateHistogramData(StationData data);
void Histogram_Serial(std::vector<float> &temperature, float minimum, float maximum);
void Histogram_Parallel(std::vector<float> &temperature, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, float minimum, float maximum);

// Global control variables
const string DATASET_PATH = "china_temp_debug.txt"; // Debug
// const string DATASET_PATH = "china_temp_short.txt"; // Development
// const string DATASET_PATH = "china_temp_large.txt"; // Final
const string KERNEL_PATH = "my_kernels.cl";

const int window_width = 1920;
const int window_height = 1080;

int histogram_bin_no = 15; // number of bins
vector<int> histogram_result = vector<int>(histogram_bin_no);
vector<int> output = vector<int>(histogram_result.size());
size_t output_size = output.size() * sizeof(float);

// Global histogram variables
vector<float> upperLimits; // upper limit for each bins
vector<int> frequencies;   // store frequency of each bins

// Main method
int main(int argc, char *argv[])
{
	// Step 0. Setup Console title
	SetConsoleTitle(L"CAT3024N Parallel Computing - Assignment | Developed by Cheah Pin Chee (0197637)");

	// Step 1. Platform Selection
	int platform_id = 1;
	int device_id = 0;
	do
	{
		refreshHeader("N/a", "N/a");

		displayMenu_PlatformAndDeviceSelection(ListPlatformsDevices());
		pair<int, int> platform_device = getPlatformAndDeviceSelection();
		platform_id = platform_device.first;
		device_id = platform_device.second;
		// Check if platform size and device size is valid
		if (platform_id >= getPlatformSize() || device_id >= getDeviceSize(platform_id))
		{
			println("Error: Invalid Platform or Device ID. Please enter a valid Platform and Device ID...");
			println("");
			pause();
		}
		else if (platform_id < 0 || device_id < 0)
		{
			// Use Default 0 0
			platform_id = 0;
			device_id = 0;
			println("Using Default Platform and Device ID...");
			pause();
			break;
		}
		else
		{
			break;
		}
	} while (true);

	// Step 2. Load the data
	StationData data = StationData();

	refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id));
	println("Loading data from file: " + DATASET_PATH);
	if (!data.Load(DATASET_PATH))
	{
		println("Error: Unable to load the data. Please check the file path '" + DATASET_PATH + "' and try again...");
		println();
		pause();
		return 0;
	}
	else
	{
		println("Data loaded successfully...");
		println();
		pause();
	}

	vector<float> &temps = data.GetTemp();
	vector<string> &stationName = data.GetStationName();
	vector<int> &months = data.GetMonth();
	vector<float> partTemp;

	// Step 3. Setup OpenCL environment
	try
	{
		// Setup the OpenCL environment
		cl::Context context = GetContext(platform_id, device_id);	// Get the context
		cl::CommandQueue queue(context, CL_QUEUE_PROFILING_ENABLE); // Get the queue
		cl::Event prof_event;										// Setup the event profiling
		cl::Program::Sources sources;								// Setup the sources

		// Load the kernel file
		refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id));
		println("Loading the kernel file: " + KERNEL_PATH);
		if (!AddSources(sources, KERNEL_PATH)) // Add the sources to the program
		{
			println("Error: Unable to load the " + KERNEL_PATH + " file. Please check the file path and try again...");
			println();
			pause();
			return 0;
		}
		else
		{
			println("Kernel file loaded successfully...");
			println();
			pause();
		}
		cl::Program program(context, sources); // Define the program with the context and sources

		refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id));
		println("Building the program...");
		try
		{
			program.build(); // Try to build the program
		}
		catch (const cl::Error &err)
		{
			println("Error: Failed to build the program. Please check the build log for more information...");
			println("\tBuild Status    \t: " + program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]));
			println("\tBuild Options   \t: " + program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]));
			println("\tBuild Log       \t: " + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(context.getInfo<CL_CONTEXT_DEVICES>()[0]));
			throw err;
			return 0;
		}
		println("Program built successfully...");
		println();
		println("OpenCL environment setup successfully...");
		println();
		pause();

		// Clock variable declaration
		clock_t startTime = 0;
		clock_t endTime = 0;

		// Step 4. Main Program Loop
		while (true)
		{
			refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id));
			// Display Main Menu
			displayMenu_Main();
			int mainMenuChoice = getMenuChoice();

			// Initialize Screen
			refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id));
			displayInfo_Operation(mainMenuChoice);

			// TODO: Rework menu
			switch (mainMenuChoice)
			{
			case 1:						 // Serial Overall Summary
				serial_Calculate(temps); // Calculate all and display
				break;
			case 2:								// Serial By Month Summary
				serial_By_Month(temps, months); // Calculate by month and display
				break;
			case 3:									   // Serial By Station Summary
				serial_By_Station(temps, stationName); // Calculate by station and display
				break;
			case 4: // Serial By Month All Station Summary
				serial_By_Month_All_Station(temps, stationName, months);
				break;

			case 5: // Serial By Station All Month Summary
				serial_By_Station_All_Month(temps, stationName, months);
				break;

			case 22:
				// std::cout << "NOTE: RUNNING ON SERIAL MODE" << endl
				// 		  << endl;
				// startTime = clock();
				// partTemp = updateHistogramData(data);
				// selectionSort(partTemp);
				// std::cout << "----------------- OVERALL SERIAL HISTOGRAM -----------------" << std::endl;
				// Histogram_Serial(partTemp, partTemp[0], partTemp[partTemp.size() - 1]);
				// endTime = clock();
				// std::cout << "------------------------------------------------------------" << std::endl;
				// std::cout << "TOTAL COMPLETION TIME: \t" << (endTime - startTime) << " ms" << std::endl;

				// system("python DrawHisto.py"); // run DrawHisto.py file

				break;
			// case 3:
			// std::cout << "NOTE: RUNNING ON PARALLEL MODE" << endl
			//   << endl;
			// Parallel(temps, context, queue, program, prof_event);
			// Parallel_Summary(temps, context, queue, program, prof_event, stationName, months);
			// break;
			// case 4:
			// std::cout << "NOTE: RUNNING ON PARALLEL MODE" << endl
			// 		  << endl;
			// startTime = clock();
			// partTemp = updateHistogramData(data);
			// Sort(partTemp, context, queue, program, prof_event);
			// std::cout << "----------------- OVERALL PARALLEL HISTOGRAM -----------------" << std::endl;
			// Histogram_Parallel(partTemp, context, queue, program, prof_event, partTemp[0], partTemp[partTemp.size() - 1]);
			// endTime = clock();
			// cout << "--------------------------------------------------------------" << std::endl;
			// cout << "TOTAL COMPLETION TIME: \t" << (endTime - startTime) << " ms" << std::endl;
			// system("python DrawHisto.py"); // run DrawHisto.py file
			// break;
			case 11:
				println("Exiting Program...");
				pause();
				return 0;
			default:
				println("Invalid input. Please enter a valid option...");
			}
			pause();
		}
	}
	catch (cl::Error err)
	{
		cerr << "ERROR: " << err.what() << ", " << getErrorString(err.err());
		pause();
	}
	return 0;
}

// Function to update histogram data based on temperature information
vector<float> updateHistogramData(StationData data)
{
	vector<float> temperatures;
	vector<float> t = data.GetTemp();
	vector<string> s = data.GetStationName();
	vector<int> m = data.GetMonth();

	temperatures = t; // copy the temperature data

	return temperatures;
}

// ++++++++++++++++++++++++++++++++++++++++ Parallel functions
// Sum Vector function
float SumVec(std::vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
{
	// Set local size to 2
	size_t local_size = 2;
	// Add padding to the vector
	int padding_size = AddPadding(temp, local_size, 0.0f);
	// Set kernel to the reduce addition kernel
	cl::Kernel kernel = cl::Kernel(program, "reduce_add_4");
	// Set return to the output from kernel execution
	float Return = KernelExecRet(kernel, temp, local_size, context, queue, true, false, false, 0.0f, 0, prof_event, "Sum Vector");
	// Return value
	return Return;
}

// Standard deviation function
float STDVec(std::vector<float> &temp, float Mean, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
{
	// Get the size of the vector
	double Size = temp.size();
	// Set local size to 2
	size_t local_size = 2;
	// Add padding to the vector
	int padding_size = AddPadding(temp, local_size, 0.0f);
	// Set kernel to the reduce standard deviation kernel
	cl::Kernel kernel = cl::Kernel(program, "reduce_STD_4");
	// Set return to the output from kernel execution
	float Return = KernelExecRet(kernel, temp, local_size, context, queue, true, true, true, Mean, padding_size, prof_event, "Standard Deviation");
	// Divide the result by the size
	Return = (Return / (Size));
	// Square root the result
	Return = sqrt(Return);
	// Return the value
	return Return;
}

// Sort function
void Sort(vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
{
	// Set local size to 32
	size_t local_size = (32);
	// Add padding to the vector
	int padding_size = AddPadding(temp, local_size, -1000000.0f);
	// Set kernel to the parallel selection kernel
	cl::Kernel kernel = cl::Kernel(program, "ParallelSelection");
	// Perform the kernel
	KernelExec(kernel, temp, local_size, context, queue, false, false, false, 0.0f, 0, prof_event, "Parallel Selection Sort");
	// Erase the padded elements at the start of the vector
	temp.erase(temp.begin(), temp.begin() + (local_size - padding_size));
}

// Function to add padding to an array
int AddPadding(std::vector<float> &temp, size_t LocalSize, float PadVal)
{
	// Set the local size
	size_t local_size = LocalSize;
	// Find the padding size
	int padding_size = temp.size() % local_size;
	// If there is padding size then
	if (padding_size)
	{
		// Create an extra vector with PadVal values
		std::vector<float> A_ext(local_size - padding_size, PadVal);
		// Append that extra vector to the input
		temp.insert(temp.end(), A_ext.begin(), A_ext.end());
	}
	// Return padding_size
	return padding_size;
}

void KernelExec(cl::Kernel kernel, std::vector<float> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event &prof_event, std::string Name)
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

	// Write all the values from temp into buffer
	queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, input_size, &temp[0], NULL, &prof_event);
	queue.enqueueFillBuffer(buffer_B, 0, 0, output_size);

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

	// Run the kernel
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(Local_Size), NULL, &prof_event);

	// Copy the result from device to host
	// Setup prof Event
	cl::Event prof_event2;
	queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, output_size, &temp[0], NULL, &prof_event2);
}

float KernelExecRet(cl::Kernel kernel, std::vector<float> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event &prof_event, std::string Name)
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

	queue.enqueueFillBuffer(buffer_B, 0, 0, output_size);

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

	// Run the kernel
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(input_elements), cl::NDRange(Local_Size), NULL, &prof_event);

	// Copy the result from device to host
	// Setup prof Event
	cl::Event prof_event2;
	queue.enqueueReadBuffer(buffer_B, CL_TRUE, 0, output_size, &B[0], NULL, &prof_event2);

	// Display kernel memory read time
	// std::cout << Name << " Kernel memory read time [ns]:" << prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_END>() - prof_event2.getProfilingInfo<CL_PROFILING_COMMAND_START>() << std::endl;
	// std::cout << GetFullProfilingInfo(prof_event2, ProfilingResolution::PROF_US) << std::endl << std::endl;

	// Return the first element of the buffer Vector B
	return B[0];
}

void Histogram_Serial(std::vector<float> &temperature, float minimum, float maximum)
{
	// Create output vector
	std::vector<int> histogram_vector(histogram_bin_no); // histogram results

	// display bins and frequency
	std::cout << "Minimum: " << minimum << ", Maximum: " << maximum << std::endl;
	std::cout << "Number of Bins: " << histogram_bin_no << ", Bin Size: " << (maximum - minimum) / histogram_bin_no << std::endl;
	float binSize = (maximum - minimum) / histogram_bin_no;
	int max_freq = 0;

	// clear vectors
	upperLimits.clear();
	frequencies.clear();

	// first element is the minimum of elements
	upperLimits.push_back(minimum);

	for (int i = 0; i < temperature.size(); i++)
	{
		float compareVal = minimum + binSize;
		int idx = 0;
		while (temperature[i] > compareVal)
		{
			compareVal += binSize; // check next range
			idx++;
		}
		if (idx == histogram_bin_no)
		{
			idx--;
		}
		histogram_vector[idx] += 1;
	}
	// save the result
	std::ofstream outputFile("histogram.txt");
	for (int i = 1; i < histogram_bin_no + 1; i++)
	{
		float binStart = minimum + ((i - 1) * binSize);
		float binEnd = minimum + (i * binSize);
		int frequency = (histogram_vector[i - 1]);
		std::cout << "Bin Range: >" << binStart << " to <=" << binEnd << ", Frequency: " << frequency << std::endl;
		outputFile << binStart << " " << binEnd << " " << frequency << " " << std::endl;

		max_freq = (frequency > max_freq) ? frequency : max_freq;
		frequencies.push_back(frequency);
		upperLimits.push_back(binEnd);
	}
	// Close the file
	outputFile.close();
	// last element is the total number of frequencies
	frequencies.push_back(max_freq);
}
