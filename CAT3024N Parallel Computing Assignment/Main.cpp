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
#include "ParallelOperation.h"

#include "Global.h"

const char *SERIAL_PYTHON_PLOT_CMD = "python Serial_HistPlot.py";
const char *PARALLEL_PYTHON_PLOT_CMD = "python Parallel_HistPlot.py";

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
			if (mainMenuChoice == 15)
			{
				println("Exiting Program...");
				pause();
				return 0;
			}

			// Initialize Screen
			refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id));
			switch (mainMenuChoice)
			{
			case 1: // Serial Overall Summary
				displayInfo_Operation(mainMenuChoice);
				serial_Overall(temps); // Calculate and display
				break;
			case 2: // Serial By Month Summary
				displayInfo_Operation(mainMenuChoice);
				serial_By_Month(temps, months); // Calculate by month and display
				break;
			case 3: // Serial By Station Summary
				displayInfo_Operation(mainMenuChoice);
				serial_By_Station(temps, stationName); // Calculate by station and display
				break;
			case 4: // Serial By Month All Station Summary
				displayInfo_Operation(mainMenuChoice);
				serial_By_Month_All_Station(temps, stationName, months);
				break;
			case 5: // Serial By Station All Month Summary
				displayInfo_Operation(mainMenuChoice);
				serial_By_Station_All_Month(temps, stationName, months);
				break;
			case 6: // Serial Full Summary
				displayInfo_Operation(1);
				serial_Overall(temps);
				displayInfo_Operation(2);
				serial_By_Month(temps, months);
				displayInfo_Operation(3);
				serial_By_Station(temps, stationName);
				displayInfo_Operation(4);
				serial_By_Month_All_Station(temps, stationName, months);
				displayInfo_Operation(mainMenuChoice);
				serial_By_Station_All_Month(temps, stationName, months);
				break;
			case 7: // Serial Histogram Summary
				displayInfo_Operation(mainMenuChoice);
				serial_Histogram(temps);
				system(SERIAL_PYTHON_PLOT_CMD); // run python plot file
				break;
			case 11: // Parallel Overall Summary
				displayInfo_Operation(mainMenuChoice);
				parallel_Overall(temps, context, queue, program, prof_event);
				break;
			case 12: // Parallel By Month Summary
				displayInfo_Operation(mainMenuChoice);
				parallel_By_Month(temps, months, context, queue, program, prof_event);
				break;
			case 13: // Parallel By Station Summary
				displayInfo_Operation(mainMenuChoice);
				parallel_By_Station(temps, stationName, context, queue, program, prof_event);
				break;
			case 14: // Parallel By Month All Station Summary
				displayInfo_Operation(mainMenuChoice);
				parallel_By_Month_All_Station(temps, stationName, months, context, queue, program, prof_event);
				break;
			case 15: // Parallel By Station All Month Summary
				displayInfo_Operation(mainMenuChoice);
				parallel_By_Station_All_Month(temps, stationName, months, context, queue, program, prof_event);
				break;
			case 16: // Parallel Full Summary
				displayInfo_Operation(11);
				parallel_Overall(temps, context, queue, program, prof_event);
				displayInfo_Operation(12);
				parallel_By_Month(temps, months, context, queue, program, prof_event);
				displayInfo_Operation(13);
				parallel_By_Station(temps, stationName, context, queue, program, prof_event);
				displayInfo_Operation(14);
				parallel_By_Month_All_Station(temps, stationName, months, context, queue, program, prof_event);
				displayInfo_Operation(15);
				parallel_By_Station_All_Month(temps, stationName, months, context, queue, program, prof_event);
				break;
			case 17: // Parallel Histogram Summary
				displayInfo_Operation(mainMenuChoice);
				parallel_Histogram(temps, context, queue, program, prof_event);
				system(PARALLEL_PYTHON_PLOT_CMD); // run python plot file
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