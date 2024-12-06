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

void setSortingAlgorithm(SORT_ALGORITHM &sortAlgorithm, int choice)
{
	switch (choice)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 101:
	case 102:
	case 103:
	case 104:
	case 105:
	case 106:
	case 107:
	case 108:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 151:
	case 152:
	case 153:
	case 154:
	case 155:
	case 156:
	case 157:
	case 158:
		sortAlgorithm = SORT_ALGORITHM::SELECTION;
		break;
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 111:
	case 112:
	case 113:
	case 114:
	case 115:
	case 116:
	case 117:
	case 118:
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 161:
	case 162:
	case 163:
	case 164:
	case 165:
	case 166:
	case 167:
	case 168:
		sortAlgorithm = SORT_ALGORITHM::MERGE;
		break;
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 121:
	case 122:
	case 123:
	case 124:
	case 125:
	case 126:
	case 127:
	case 128:
	case 71:
	case 72:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
	case 171:
	case 172:
	case 173:
	case 174:
	case 175:
	case 176:
	case 177:
	case 178:
		sortAlgorithm = SORT_ALGORITHM::BUBBLE;
		break;
	default:
		sortAlgorithm = SORT_ALGORITHM::SELECTION;
		break;
	}
}

// Main method
int main(int argc, char *argv[])
{
	int binSize = 0; // Default bin size

	// Step 0. Setup Console title
	SetConsoleTitle(L"CAT3024N Parallel Computing - Assignment | Developed by Cheah Pin Chee (0197637)");

	// Step 1. Platform Selection
restart:
	int platform_id = 0; // Default Platform ID
	int device_id = 0;	 // Default Device ID
	do
	{
		refreshHeader("N/a", "N/a", binSize); // Displaying

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
loadData:
	StationData data = StationData();
	clock_t dataStartTime, dataEndTime;
	do
	{
		string dataPath;
		refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id), binSize); // Refresh the header
		displayMenu_DatasetSelection();
		int datasetChoice = getInput();
		if (datasetChoice == 0)
			dataPath = "china_temp_debug.txt"; // Debug
		else if (datasetChoice == 1)
			dataPath = "china_temp_short.txt"; // Development
		else if (datasetChoice == 2)
			dataPath = "china_temp_large.txt"; // Final
		else
			dataPath = "china_temp_short.txt";
		println("Loading data from file: " + dataPath); // Display the loading data message
		dataStartTime = clock();						// Start the clock
		if (!data.Load(dataPath))
		{
			println("Error: Unable to load the data. Please check the file path '" + dataPath + "' and try again...");
			println();
			pause();
			continue;
		}
		else
		{
			dataEndTime = clock();					 // End the clock
			print("Data loaded successfully... | "); // Display the success message
			break;
		}
	} while (true);

	// If the data is loaded successfully, get the data into vectors
	vector<float> &temps = data.GetTemp();
	vector<string> &stationName = data.GetStationName();
	vector<int> &months = data.GetMonth();
	vector<int> &years = data.GetYear();

	println("Time taken to load data: " + to_string(dataEndTime - dataStartTime) + "ms" + " | " + to_string(temps.size()) + " records loaded...");
	println();
	pause();

// Step 3. Set Bin Size
setBinSize:
	do
	{
		refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id), binSize);
		displayMenu_HistogramBinSizeSetting();
		int binSizeChoice = getInput();
		if (binSizeChoice >= 1 && binSizeChoice <= 1000)
		{
			binSize = binSizeChoice;
			break;
		}
		else
		{
			println("Error: Invalid bin size. Please enter a valid bin size between 1 and 1000...");
			println();
			pause();
		}
	} while (true);

	// Step 4. Setup OpenCL environment
	try
	{
		// Setup the OpenCL environment
		cl::Context context = GetContext(platform_id, device_id);	// Get the context
		cl::CommandQueue queue(context, CL_QUEUE_PROFILING_ENABLE); // Get the queue
		cl::Event prof_event;										// Setup the event profiling
		cl::Program::Sources sources;								// Setup the sources

		// Load the kernel file
		refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id), binSize);
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

		refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id), binSize);
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

		// Step 5. Main Program Loop
		while (true)
		{
			refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id), binSize);
			// Display Main Menu
			displayMenu_Main();
			int mainMenuChoice = getInput();
			if (mainMenuChoice == -3)
			{
				println("Exiting Program...");
				pause();
				return 0;
			}
			else if (mainMenuChoice == -2)
			{
				println("Directing to Bin Size Setting...");
				pause();
				goto setBinSize;
				continue;
			}
			else if (mainMenuChoice == -1)
			{
				println("Restarting program...");
				pause();
				goto restart;
				continue;
			}

			// Initialize Screen
			refreshHeader(GetPlatformName(platform_id), GetDeviceName(platform_id, device_id), binSize);
			displayInfo_Operation(mainMenuChoice);
			// Process sorting method
			SORT_ALGORITHM sortAlgorithm;
			setSortingAlgorithm(sortAlgorithm, mainMenuChoice); // Set the sorting algorithm

			// Main Menu Choice
			switch (mainMenuChoice)
			{
			case 1:									  // Serial Overall Summary
			case 11:								  // Serial Overall Summary
			case 21:								  // Serial Overall Summary
				serial_Overall(temps, sortAlgorithm); // Calculate and display
				break;
			case 2:											 // Serial By Year Summary
			case 12:										 // Serial By Year Summary
			case 22:										 // Serial By Year Summary
				serial_By_Year(temps, years, sortAlgorithm); // Calculate by year and display
				break;
			case 3:											   // Serial By Month Summary
			case 13:										   // Serial By Month Summary
			case 23:										   // Serial By Month Summary
				serial_By_Month(temps, months, sortAlgorithm); // Calculate by month and display
				break;
			case 4:													  // Serial By Station Summary
			case 14:												  // Serial By Station Summary
			case 24:												  // Serial By Station Summary
				serial_By_Station(temps, stationName, sortAlgorithm); // Calculate by station and display
				break;
			case 5:
			case 15:
			case 25:
				serial_By_Year_All_Station(temps, years, stationName, sortAlgorithm);
				break;
			case 6:	 // Serial By Month All Station Summary
			case 16: // Serial By Month All Station Summary
			case 26: // Serial By Month All Station Summary
				serial_By_Month_All_Station(temps, stationName, months, sortAlgorithm);
				break;
			case 7:	 // Serial By Station All Year
			case 17: // Serial By Station All Year
			case 27: // Serial By Station All Year
				serial_By_Station_All_Year(temps, stationName, years, sortAlgorithm);
				break;
			case 8:	 // Serial By Station All Month Summary
			case 18: // Serial By Station All Month Summary
			case 28: // Serial By Station All Month Summary
				serial_By_Station_All_Month(temps, stationName, months, sortAlgorithm);
				break;

			case 51: // Serial Histogram Overall
			case 61: // Serial Histogram Overall
			case 71: // Serial Histogram Overall
				serial_Histogram(temps, "Serial_Histogram_Overall.csv", binSize, sortAlgorithm);
				break;
			case 52: // Serial Histogram By Year
			case 62: // Serial Histogram By Year
			case 72: // Serial Histogram By Year
				serial_Histogram_By_Year(temps, years, binSize, sortAlgorithm);
				break;
			case 53: // Serial Histogram By Month
			case 63: // Serial Histogram By Month
			case 73: // Serial Histogram By Month
				serial_Histogram_By_Month(temps, months, binSize, sortAlgorithm);
				break;
			case 54: // Serial Histogram By Station
			case 64: // Serial Histogram By Station
			case 74: // Serial Histogram By Station
				serial_Histogram_By_Station(temps, stationName, binSize, sortAlgorithm);
				break;
			case 55: // Serial Histogram By Year All Station
			case 65: // Serial Histogram By Year All Station
			case 75: // Serial Histogram By Year All Station
				serial_Histogram_By_Year_All_Station(temps, years, stationName, binSize, sortAlgorithm);
				break;
			case 56: // Serial Histogram By Month All Station
			case 66: // Serial Histogram By Month All Station
			case 76: // Serial Histogram By Month All Station
				serial_Histogram_By_Month_All_Station(temps, stationName, months, binSize, sortAlgorithm);
				break;
			case 57: // Serial Histogram By Station All Year
			case 67: // Serial Histogram By Station All Year
			case 77: // Serial Histogram By Station All Year
				serial_Histogram_By_Station_All_Year(temps, years, stationName, binSize, sortAlgorithm);
				break;
			case 58: // Serial Histogram By Station All Month
			case 68: // Serial Histogram By Station All Month
			case 78: // Serial Histogram By Station All Month
				serial_Histogram_By_Station_All_Month(temps, stationName, months, binSize, sortAlgorithm);
				break;

			case 101: // Parallel Overall Summary
			case 111: // Parallel Overall Summary
			case 121: // Parallel Overall Summary
				parallel_Overall(temps, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 102: // Parallel By Year Summary
			case 112: // Parallel By Year Summary
			case 122: // Parallel By Year Summary
				parallel_By_Year(temps, years, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 103: // Parallel By Month Summary
			case 123: // Parallel By Month Summary
			case 133: // Parallel By Month Summary
				parallel_By_Month(temps, months, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 104: // Parallel By Station Summary
			case 114: // Parallel By Station Summary
			case 124: // Parallel By Station Summary
				parallel_By_Station(temps, stationName, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 105: // Parallel By Year All Station Summary
			case 115: // Parallel By Year All Station Summary
			case 125: // Parallel By Year All Station Summary
				parallel_By_Year_All_Station(temps, years, stationName, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 106: // Parallel By Month All Station Summary
			case 116: // Parallel By Month All Station Summary
			case 126: // Parallel By Month All Station Summary
				parallel_By_Month_All_Station(temps, stationName, months, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 107: // Parallel By Station All Year Summary
			case 117: // Parallel By Station All Year Summary
			case 127: // Parallel By Station All Year Summary
				parallel_By_Station_All_Year(temps, years, stationName, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 108: // Parallel By Station All Month Summary
			case 118: // Parallel By Station All Month Summary
			case 128: // Parallel By Station All Month Summary
				parallel_By_Station_All_Month(temps, stationName, months, sortAlgorithm, context, queue, program, prof_event);
				break;

			case 151: // Parallel Histogram Overall
			case 161: // Parallel Histogram Overall
			case 171: // Parallel Histogram Overall
				parallel_Histogram(temps, "Parallel_Histogram_Overall.csv", binSize, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 152: // Parallel Histogram By Year
			case 162: // Parallel Histogram By Year
			case 172: // Parallel Histogram By Year
				parallel_Histogram_By_Year(temps, years, binSize, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 153: // Parallel Histogram By Month
			case 163: // Parallel Histogram By Month
			case 173: // Parallel Histogram By Month
				parallel_Histogram_By_Month(temps, months, binSize, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 154: // Parallel Histogram By Station
			case 164: // Parallel Histogram By Station
			case 174: // Parallel Histogram By Station
				parallel_Histogram_By_Station(temps, stationName, binSize, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 155: // Parallel Histogram By Year All Station
			case 165: // Parallel Histogram By Year All Station
			case 175: // Parallel Histogram By Year All Station
				parallel_Histogram_By_Year_All_Station(temps, years, stationName, binSize, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 156: // Parallel Histogram By Month All Station
			case 166: // Parallel Histogram By Month All Station
			case 176: // Parallel Histogram By Month All Station
				parallel_Histogram_By_Month_All_Station(temps, stationName, months, binSize, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 157: // Parallel Histogram By Station All Year
			case 167: // Parallel Histogram By Station All Year
			case 177: // Parallel Histogram By Station All Year
				parallel_Histogram_By_Station_All_Year(temps, years, stationName, binSize, sortAlgorithm, context, queue, program, prof_event);
				break;
			case 158: // Parallel Histogram By Station All Month
			case 168: // Parallel Histogram By Station All Month
			case 178: // Parallel Histogram By Station All Month
				parallel_Histogram_By_Station_All_Month(temps, stationName, months, binSize, sortAlgorithm, context, queue, program, prof_event);
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