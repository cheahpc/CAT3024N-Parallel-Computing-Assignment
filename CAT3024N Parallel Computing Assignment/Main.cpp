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
#include "StationData.h"
#include "SerialOperation.h"
#include "ParallelOperation.h"

#include "Global.h"

// Main method
int main(int argc, char *argv[])
{
	// Step 0. Setup Console title
	SetConsoleTitle(L"CAT3024N Parallel Computing - Assignment | Developed by Cheah Pin Chee (0197637)");

	// Step 1. Platform Selection
	int platform_id = 0; // Default Platform ID
	int device_id = 0;	 // Default Device ID
	do
	{
		refreshHeader("N/a", "N/a"); // Displaying

		displayMenu_PlatformAndDeviceSelection(ListPlatformsDevices());	  // Display the platform and device selection menu
		pair<int, int> platform_device = getPlatformAndDeviceSelection(); // Get the platform and device selection
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

	refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id)); // Refresh the header
	println("Loading data from file: " + DATASET_PATH);									// Display the loading data message
	clock_t dataStartTime = clock();													// Start the clock
	clock_t dataEndTime;
	if (!data.Load(DATASET_PATH))
	{
		println("Error: Unable to load the data. Please check the file path '" + DATASET_PATH + "' and try again...");
		println();
		pause();
		return 0;
	}
	else
	{
		dataEndTime = clock();					 // End the clock
		print("Data loaded successfully... | "); // Display the success message
	}

	// If the data is loaded successfully, get the data into vectors
	vector<float> &temps = data.GetTemp();
	vector<string> &stationName = data.GetStationName();
	vector<int> &months = data.GetMonth();
	vector<int> &years = data.GetYear();

	println("Time taken to load data: " + to_string(dataEndTime - dataStartTime) + "ms" + " | " + to_string(temps.size()) + " records loaded...");
	println();
	pause();

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
		// If the kernel file is loaded successfully, build the program
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
		// If the program is built successfully, display the success message
		println("Program built successfully...");
		println();
		println("OpenCL environment setup successfully...");
		println();
		pause();

		// Step 4. Main Program Loop
		while (true)
		{
			refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id));
			// Display Main Menu
			displayMenu_Main();
			int mainMenuChoice = getMenuChoice();
			if (mainMenuChoice == -1)
			{
				println("Exiting Program...");
				pause();
				return 0;
			}

			// Initialize Screen
			refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id));
			displayInfo_Operation(mainMenuChoice);

			switch (mainMenuChoice)
			{
			case 1:					   // Serial Overall Summary
				serial_Overall(temps); // Calculate and display
				break;
			case 2:							  // Serial By Year Summary
				serial_By_Year(temps, years); // Calculate by year and display
				break;
			case 3:								// Serial By Month Summary
				serial_By_Month(temps, months); // Calculate by month and display
				break;
			case 4:									   // Serial By Station Summary
				serial_By_Station(temps, stationName); // Calculate by station and display
				break;
			case 5:
				serial_By_Year_All_Station(temps, years, stationName);
				break;
			case 6: // Serial By Month All Station Summary
				serial_By_Month_All_Station(temps, stationName, months);
				break;
			case 7: // Serial By Station All Year
				serial_By_Station_All_Year(temps, stationName, years);
				break;
			case 8: // Serial By Station All Month Summary
				serial_By_Station_All_Month(temps, stationName, months);
				break;
			case 9: // Serial Full Summary
				displayInfo_Operation(1);
				serial_Overall(temps);
				displayInfo_Operation(2);
				serial_By_Year(temps, years);
				displayInfo_Operation(3);
				serial_By_Month(temps, months);
				displayInfo_Operation(4);
				serial_By_Station(temps, stationName);
				displayInfo_Operation(5);
				serial_By_Year_All_Station(temps, years, stationName);
				displayInfo_Operation(6);
				serial_By_Month_All_Station(temps, stationName, months);
				displayInfo_Operation(7);
				serial_By_Station_All_Year(temps, stationName, years);
				displayInfo_Operation(8);
				serial_By_Station_All_Month(temps, stationName, months);
				break;
			case 51: // Serial Histogram Overall
				serial_Histogram(temps, "Serial_Histogram_Overall.csv");
				break;
			case 52: // Serial Histogram By Year
				serial_Histogram_By_Year(temps, years);
				break;
			case 53: // Serial Histogram By Month
				serial_Histogram_By_Month(temps, months);
				break;
			case 54: // Serial Histogram By Station
				serial_Histogram_By_Station(temps, stationName);
				break;
			case 55: // Serial Histogram By Year All Station
				serial_Histogram_By_Year_All_Station(temps, years, stationName);
				break;
			case 56: // Serial Histogram By Month All Station
				serial_Histogram_By_Month_All_Station(temps, stationName, months);
				break;
			case 57: // Serial Histogram By Station All Year
				serial_Histogram_By_Station_All_Year(temps, years, stationName);
				break;
			case 58: // Serial Histogram By Station All Month
				serial_Histogram_By_Station_All_Month(temps, stationName, months);
				break;
			case 59: // Serial Full Histogram
				displayInfo_Operation(51);
				serial_Histogram(temps, "Serial_Histogram_Overall.csv");
				displayInfo_Operation(52);
				serial_Histogram_By_Year(temps, years);
				displayInfo_Operation(53);
				serial_Histogram_By_Month(temps, months);
				displayInfo_Operation(54);
				serial_Histogram_By_Station(temps, stationName);
				displayInfo_Operation(55);
				serial_Histogram_By_Year_All_Station(temps, years, stationName);
				displayInfo_Operation(56);
				serial_Histogram_By_Month_All_Station(temps, stationName, months);
				displayInfo_Operation(57);
				serial_Histogram_By_Station_All_Year(temps, years, stationName);
				displayInfo_Operation(58);
				serial_Histogram_By_Station_All_Month(temps, stationName, months);
				break;

			case 101: // Parallel Overall Summary
				parallel_Overall(temps, context, queue, program, prof_event);
				break;
			case 102: // Parallel By Year Summary
				parallel_By_Year(temps, years, context, queue, program, prof_event);
				break;
			case 103: // Parallel By Month Summary
				parallel_By_Month(temps, months, context, queue, program, prof_event);
				break;
			case 104: // Parallel By Station Summary
				parallel_By_Station(temps, stationName, context, queue, program, prof_event);
				break;
			case 105: // Parallel By Year All Station Summary
				parallel_By_Year_All_Station(temps, years, stationName, context, queue, program, prof_event);
				break;
			case 106: // Parallel By Month All Station Summary
				parallel_By_Month_All_Station(temps, stationName, months, context, queue, program, prof_event);
				break;
			case 107: // Parallel By Station All Year Summary
				parallel_By_Station_All_Year(temps, years, stationName, context, queue, program, prof_event);
				break;
			case 108: // Parallel By Station All Month Summary
				parallel_By_Station_All_Month(temps, stationName, months, context, queue, program, prof_event);
				break;
			case 109: // Parallel Full Summary
				displayInfo_Operation(101);
				parallel_Overall(temps, context, queue, program, prof_event);
				displayInfo_Operation(102);
				parallel_By_Year(temps, years, context, queue, program, prof_event);
				displayInfo_Operation(103);
				parallel_By_Month(temps, months, context, queue, program, prof_event);
				displayInfo_Operation(104);
				parallel_By_Station(temps, stationName, context, queue, program, prof_event);
				displayInfo_Operation(105);
				parallel_By_Year_All_Station(temps, years, stationName, context, queue, program, prof_event);
				displayInfo_Operation(106);
				parallel_By_Month_All_Station(temps, stationName, months, context, queue, program, prof_event);
				displayInfo_Operation(107);
				parallel_By_Station_All_Year(temps, years, stationName, context, queue, program, prof_event);
				displayInfo_Operation(108);
				parallel_By_Station_All_Month(temps, stationName, months, context, queue, program, prof_event);
				break;
			case 151: // Parallel Histogram Overall
				parallel_Histogram(temps, "Parallel_Histogram_Overall.csv", context, queue, program, prof_event);
				break;
			case 152: // Parallel Histogram By Year
				parallel_Histogram_By_Year(temps, years, context, queue, program, prof_event);
				break;
			case 153: // Parallel Histogram By Month
				parallel_Histogram_By_Month(temps, months, context, queue, program, prof_event);
				break;
			case 154: // Parallel Histogram By Station
				parallel_Histogram_By_Station(temps, stationName, context, queue, program, prof_event);
				break;
			case 155: // Parallel Histogram By Year All Station
				parallel_Histogram_By_Year_All_Station(temps, years, stationName, context, queue, program, prof_event);
				break;
			case 156: // Parallel Histogram By Month All Station
				parallel_Histogram_By_Month_All_Station(temps, stationName, months, context, queue, program, prof_event);
				break;
			case 157: // Parallel Histogram By Station All Year
				parallel_Histogram_By_Station_All_Year(temps, years, stationName, context, queue, program, prof_event);
				break;
			case 158: // Parallel Histogram By Station All Month
				parallel_Histogram_By_Station_All_Month(temps, stationName, months, context, queue, program, prof_event);
				break;
			case 159: // Parallel Full Histogram
				displayInfo_Operation(151);
				parallel_Histogram(temps, "Parallel_Histogram_Overall.csv", context, queue, program, prof_event);
				displayInfo_Operation(152);
				parallel_Histogram_By_Year(temps, years, context, queue, program, prof_event);
				displayInfo_Operation(153);
				parallel_Histogram_By_Month(temps, months, context, queue, program, prof_event);
				displayInfo_Operation(154);
				parallel_Histogram_By_Station(temps, stationName, context, queue, program, prof_event);
				displayInfo_Operation(155);
				parallel_Histogram_By_Year_All_Station(temps, years, stationName, context, queue, program, prof_event);
				displayInfo_Operation(156);
				parallel_Histogram_By_Month_All_Station(temps, stationName, months, context, queue, program, prof_event);
				displayInfo_Operation(157);
				parallel_Histogram_By_Station_All_Year(temps, years, stationName, context, queue, program, prof_event);
				displayInfo_Operation(158);
				parallel_Histogram_By_Station_All_Month(temps, stationName, months, context, queue, program, prof_event);
				break;
			default:
				println("Invalid input. Please enter a valid option...");
				break;
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