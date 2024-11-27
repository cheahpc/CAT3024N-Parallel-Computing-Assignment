#define __CL_ENABLE_EXCEPTIONS

#include <CL/openCL.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

#include "Menu.h"
#include "Utils.h"
#include "StationData.h"
#include "SerialStatistics.h"

// Define myType
typedef float myType;

// Function Declaration
myType SumVec(std::vector<myType> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event);
myType STDVec(std::vector<myType> &temp, myType Mean, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event);
void Sort(std::vector<myType> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event);
int AddPadding(std::vector<myType> &temp, size_t LocalSize, float PadVal);
void KernelExec(cl::Kernel kernel, std::vector<myType> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event &prof_event, std::string Name);
float KernelExecRet(cl::Kernel kernel, std::vector<myType> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event &prof_event, std::string Name);
void selectionSort(std::vector<myType> &Values);

void Serial(std::vector<myType> &Values);
void Serial_Summary(std::vector<float> &temp, std::vector<string> &stationName, std::vector<int> &month);
void Parallel(std::vector<float> &Values, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event);
void Parallel_Summary(std::vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, std::vector<string> &stationName, std::vector<int> &month);

std::vector<float> updateHistogramData(StationData data);
void Histogram_Serial(std::vector<float> &temperature, float minimum, float maximum);
void Histogram_Parallel(std::vector<float> &temperature, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, float minimum, float maximum);

void displayMenu();

// global control variables
bool serial_displayOverall = true;
bool parallel_displayOverall = true;
const string dataset_path = "china_temp_short.txt";
const string stationsList[5] = {"Taiyuan", "Chongqing", "Beijing", "Lanzhou", "Urumqi"};
const string monthsList[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
int histogram_bin_no = 15; // number of bins
std::vector<int> histogram_result = std::vector<int>(histogram_bin_no);
std::vector<int> output = std::vector<int>(histogram_result.size());
size_t output_size = output.size() * sizeof(float);

// Global histogram variables
std::vector<float> upperLimits; // upper limit for each bins
std::vector<int> frequencies;	// store frequency of each bins

SerialStatistics SStats = SerialStatistics();

// Print help function
void print_help()
{
	// Display help message
	std::cout << "Application usage:" << std::endl;
	std::cout << "  -p : select platform " << std::endl;
	std::cout << "  -d : select device" << std::endl;
	std::cout << "  -l : list all platforms and devices" << std::endl;
	std::cout << "  -h : print this message" << std::endl;
}

// Main method
int main(int argc, char **argv)
{
	// Load Temperature Data
	StationData data = StationData();
	data.Load("china_temp_short.txt");
	// Get the temperature data (degree celcius) from the temperature
	vector<float> &temps = data.GetTemp();
	// Get the station name from the temperature data
	vector<string> &stationName = data.GetStationName();
	// Get month from temperature data
	vector<int> &months = data.GetMonth();
	vector<float> partTemp;

	// Handle command line options such as device selection, verbosity, etc.
	int platform_id = 1;
	int device_id = 0;
	// Loop over arguments and perform functions
	for (int i = 1; i < argc; i++)
	{
		if ((strcmp(argv[i], "-p") == 0) && (i < (argc - 1)))
		{
			platform_id = atoi(argv[++i]);
		}
		else if ((strcmp(argv[i], "-d") == 0) && (i < (argc - 1)))
		{
			device_id = atoi(argv[++i]);
		}
		else if (strcmp(argv[i], "-l") == 0)
		{
			std::cout << ListPlatformsDevices() << std::endl;
		}
		else if (strcmp(argv[i], "-h") == 0)
		{
			print_help();
		}
	}
	// Try to get all the relevant information ready
	try
	{
		// Get the context
		cl::Context context = GetContext(platform_id, device_id);
		// Display what platform and device the code is running on
		std::cout << "Running on " << GetPlatformName(platform_id) << ", " << GetDeviceName(platform_id, device_id) << std::endl;
		// Get the queue
		cl::CommandQueue queue(context, CL_QUEUE_PROFILING_ENABLE);
		// Setup the sources
		cl::Program::Sources sources;
		// Link to the kernels.cl document
		AddSources(sources, "my_kernels.cl");
		// Define the program with the context and sources
		cl::Program program(context, sources);
		// Setup prof Event
		cl::Event prof_event;

		// Try to build the program
		try
		{
			program.build();
		}
		catch (const cl::Error &err)
		{
			// Else display error messages
			cout << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			cout << "Build Options:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			cout << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			throw err;
		}

		// Clock variable declaration
		float startTime = 0;
		float endTime = 0;

		while (true)
		{
			system("cls");
			displayMenu();
			int choice;
			std::cout << "Enter option: ";
			std::cin >> choice;

			if (choice > 0 && choice < 5)
			{
				system("cls");
				std::cout << "Loading..." << endl;
			}

			switch (choice)
			{
			case 1:
				std::cout << "NOTE: RUNNING ON SERIAL MODE" << endl
						  << endl;
				Serial(temps);
				Serial_Summary(temps, stationName, months);
				break;
			case 2:
				std::cout << "NOTE: RUNNING ON SERIAL MODE" << endl
						  << endl;
				startTime = clock();
				partTemp = updateHistogramData(data);
				selectionSort(partTemp);
				std::cout << "----------------- OVERALL SERIAL HISTOGRAM -----------------" << std::endl;
				Histogram_Serial(partTemp, partTemp[0], partTemp[partTemp.size() - 1]);
				endTime = clock();
				std::cout << "------------------------------------------------------------" << std::endl;
				std::cout << "TOTAL COMPLETION TIME: \t" << (endTime - startTime) << " ms" << std::endl;

				system("python DrawHisto.py"); // run DrawHisto.py file

				break;
			case 3:
				std::cout << "NOTE: RUNNING ON PARALLEL MODE" << endl
						  << endl;
				Parallel(temps, context, queue, program, prof_event);
				Parallel_Summary(temps, context, queue, program, prof_event, stationName, months);
				break;
			case 4:
				std::cout << "NOTE: RUNNING ON PARALLEL MODE" << endl
						  << endl;
				startTime = clock();
				partTemp = updateHistogramData(data);
				Sort(partTemp, context, queue, program, prof_event);
				std::cout << "----------------- OVERALL PARALLEL HISTOGRAM -----------------" << std::endl;
				Histogram_Parallel(partTemp, context, queue, program, prof_event, partTemp[0], partTemp[partTemp.size() - 1]);
				endTime = clock();
				std::cout << "--------------------------------------------------------------" << std::endl;
				std::cout << "TOTAL COMPLETION TIME: \t" << (endTime - startTime) << " ms" << std::endl;
				system("python DrawHisto.py"); // run DrawHisto.py file
				break;
			case 5:
				std::cout << endl;
				std::cout << "Exiting Program..." << std::endl;
				system("pause");
				return 0;
			default:
				std::cout << "Invalid input. Please enter a valid command..." << std::endl;
			}

			std::cout << endl;
			system("pause");
		}
	}
	catch (cl::Error err)
	{
		std::cerr << "ERROR: " << err.what() << ", " << getErrorString(err.err()) << std::endl;
	}

	system("pause");
	return 0;
}

// Main Menu
void displayMenu()
{
	// Header Information (Console)
	displayHeader();
	displayMainMenu();
}

// Function to update histogram data based on temperature information
std::vector<float> updateHistogramData(StationData data)
{
	std::vector<float> temperatures;
	std::vector<float> t = data.GetTemp();
	std::vector<string> s = data.GetStationName();
	std::vector<int> m = data.GetMonth();

	temperatures = t;

	return temperatures;
}

// Function to perform selection sort on a vector of myType elements
// This is intended to be used in serial execution
// This sorting algorithm processes the elements one after another, in sequence
void selectionSort(std::vector<myType> &Values)
{
	int min_id_x;
	// One by one move boundary of unsorted array
	for (int i = 0; i < Values.size() - 1; i++)
	{
		min_id_x = i;
		for (int j = i + 1; j < Values.size(); j++)
		{
			// Store the index of minimum element in unsorted array
			if (Values[j] < Values[min_id_x])
			{
				min_id_x = j;
			}
		}
		// Swap the found min element with the bigger element
		if (min_id_x != i)
		{
			// Sorting or Swapping elements
			myType temp = Values[min_id_x]; // Put the min value into temporary var
			Values[min_id_x] = Values[i];	// Put the bigger element to the min value's element array
			Values[i] = temp;				// Put the min element to go into the bigger element array
		}
	}
}

void Display_Data(int size, float sum, float mean, float sDeviation, float min, float max, float median, float Q1, float Q3, bool displayType, float startTime, float endTime)
{
	// Display
	if (displayType)
	{ // if is summary data
		std::cout << "==================================[OVERALL RESULT]==================================" << std::endl;
		// Display in multiple rows
		std::cout << "The Min: \t\t" << min << std::endl;
		std::cout << "The Max: \t\t" << max << std::endl;
		std::cout << "The Mean: \t\t" << mean << std::endl;
		std::cout << "The SD: \t\t" << sDeviation << std::endl;
		std::cout << "The MV: \t\t" << median << std::endl;
		std::cout << "The Q1: \t\t" << Q1 << std::endl;
		std::cout << "The Q3: \t\t" << Q3 << std::endl;
		std::cout << std::endl;
		std::cout << "Total Records: \t\t" << size << std::endl;
		std::cout << "Time taken: \t\t" << (endTime - startTime) << " ms" << std::endl;
		std::cout << std::endl
				  << std::endl;
	}
	else
	{
		// Display data in single row
		std::cout << min << "\t";
		std::cout << max << "\t";
		std::cout << mean << "\t";
		std::cout << sDeviation << "\t";
		std::cout << median << "\t";
		std::cout << Q1 << "\t";
		std::cout << Q3 << "\t";
		std::cout << size << "\t";
		std::cout << (endTime - startTime) << " ms" << std::endl;
	}
}

void Serial(std::vector<myType> &values)
{
	// Start Counting
	float startTime = clock();

	std::vector<myType> temperature = values;

	// Get the size of the vector
	int Size = temperature.size();
	// Perform selection sort
	selectionSort(temperature);

	// Calculate the Sum
	float Sum = SStats.Sum(temperature);
	// Calculate the mean
	float Mean = Sum / (Size);
	// Calculate the Standard Deviation
	float sDeviation = SStats.StandardDeviation(temperature);
	// Calculate the Min
	float MIN = temperature[0];
	// Calculate the Max
	float MAX = temperature[Size - 1];
	// Calculate the Median
	float Median = SStats.GetMedianValue(temperature);
	// Calculate the First Quartile
	float Q1 = SStats.FirstQuartile(temperature);
	// Calculate the Third Quartile
	float Q3 = SStats.ThirdQuartile(temperature);

	// End counting
	float endTime = clock();

	// Display Data
	Display_Data(Size, Sum, Mean, sDeviation, MIN, MAX, Median, Q1, Q3, serial_displayOverall, startTime, endTime);
}

void Serial_Summary(std::vector<float> &temp, std::vector<string> &stationName, std::vector<int> &month)
{
	serial_displayOverall = false;

	// Start counting for Station
	float startTime = clock();

	// Display results for stations
	std::cout << "=================================[STATION RESULT]===================================" << std::endl;
	std::cout << "STATION    \tMIN \tMAX \tMEAN \tSD \tMEDIAN \t1Q \t3Q \tSIZE \tTIME" << std::endl;
	std::cout << "====================================================================================" << std::endl;

// Part of temparatures belong to a specific station
std:
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
				// if matched, continue adding the temparature to partTemp
				if (stationName[i] == stationName[i + 1])
				{
					partTemp.insert(partTemp.begin(), temp[i]);
				}
				else
				{
					partTemp.insert(partTemp.begin(), temp[i]);
					std::cout << stationName[i] << "  \t";
					Serial(partTemp);
					partTemp.clear(); // Reset the partTemp
				}
			}
			else
			{
				// Last temperature data
				partTemp.insert(partTemp.begin(), temp[i]);
				std::cout << stationName[i] << " \t\t";
				Serial(partTemp);
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

	// Start Counting for Months
	startTime = clock();

	// Display the results for 12 months
	std::cout << "=================================[MONTHS RESULT]====================================" << std::endl;
	std::cout << "MONTH    \tMIN \tMAX \tMEAN \tSD \tMEDIAN \t1Q \t3Q \tSIZE \tTIME" << std::endl;
	std::cout << "====================================================================================" << std::endl;

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
		// Trigger the serial function to display
		Serial(temp2D[i]);
	}

	// End Counting
	endTime = clock();

	std::cout << std::endl;
	std::cout << "TOTAL COMPLETION TIME: \t" << (endTime - startTime) << " ms" << std::endl
			  << std::endl
			  << std::endl;
	;

	serial_displayOverall = true;
}

void Parallel(std::vector<float> &Values, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
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
	float Median = SStats.GetMedianValue(temperature);
	// Calculate the 1st Quartile
	float Q1 = SStats.FirstQuartile(temperature);
	// Calculate the 3rd Quartile
	float Q3 = SStats.ThirdQuartile(temperature);

	// End Counting
	float endTime = clock();

	// Display Data
	Display_Data(Size, Sum, Mean, sDeviation, MIN, MAX, Median, Q1, Q3, parallel_displayOverall, startTime, endTime);
}

void Parallel_Summary(std::vector<float> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event, std::vector<string> &stationName, std::vector<int> &month)
{
	parallel_displayOverall = false; // Set global parallel display type

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
	std::cout << "==================================[MONTHS RESULT]===================================" << std::endl;
	std::cout << "MONTH    \tMIN \tMAX \tMEAN \tSD \tMEDIAN \t1Q \t3Q \tSIZE \tTIME" << std::endl;
	std::cout << "====================================================================================" << std::endl;

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

// Sum Vector function
myType SumVec(std::vector<myType> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
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
myType STDVec(std::vector<myType> &temp, myType Mean, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
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
void Sort(std::vector<myType> &temp, cl::Context context, cl::CommandQueue queue, cl::Program program, cl::Event &prof_event)
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
int AddPadding(std::vector<myType> &temp, size_t LocalSize, float PadVal)
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

void KernelExec(cl::Kernel kernel, std::vector<myType> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event &prof_event, std::string Name)
{
	// Get the size of the vector
	double Size = temp.size();

	// Get the number of input elements
	size_t input_elements = temp.size();
	// Size in bytes of the input vector
	size_t input_size = temp.size() * sizeof(myType);

	// Define Output vector B
	std::vector<myType> B(input_elements);
	// Get the size in bytes of the output vector
	size_t output_size = B.size() * sizeof(myType);

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
		kernel.setArg(2, cl::Local(Local_Size * sizeof(myType))); // Local memory size
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

float KernelExecRet(cl::Kernel kernel, std::vector<myType> &temp, size_t Local_Size, cl::Context context, cl::CommandQueue queue, bool Two, bool Three, bool Four, float FThree, int IFour, cl::Event &prof_event, std::string Name)
{
	// Get the size of the vector
	double Size = temp.size();

	// Get the number of input elements
	size_t input_elements = temp.size();
	// Size in bytes of the input vector
	size_t input_size = temp.size() * sizeof(myType);

	// Define Output vector B
	std::vector<myType> B(input_elements);
	// Get the size in bytes of the output vector
	size_t output_size = B.size() * sizeof(myType);

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
		kernel.setArg(2, cl::Local(Local_Size * sizeof(myType))); // Local memory size
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

	size_t vector_elements = temperature.size();		   // number of elements
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