#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

void print(string str)
{
    cout << str;
}

void println(string str = "")
{
    cout << str << endl;
}

void displayHeader()
{
    println("=================================================================================================================================================================");
    println("| Cheah Pin Chee 0197637 |                                                                                                                               Ver.01 |");
    println("|-------------------------                                   CAT3024N Parallel Computing - Assignment                                                           |");
    println("|                                                                                                                                                               |");
    println("=================================================================================================================================================================");
    println("");
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Main Menu
void displayMenu_Main()
{
    println();
    println("=================================================================================================================================================================");
    println("|                                                                           Main Menu                                                                           |");
    println("=================================================================================================================================================================");
    // println("|            Serial Operations (xx)             |           Parallel Operations (xxx)           |   Description                                                 |");
    // println("|-----------------------------------------------|-----------------------------------------------|---------------------------------------------------------------|");
    // println("|   Selection   |     Merge     |    Bubble     |   Selection   |    Bitonic    |    Odd Even   |   Summary                                                     |");
    // println("|---------------|---------------|---------------|---------------|---------------|---------------|---------------------------------------------------------------|");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       1       |      11       |      21       |      101      |      111      |      121      |   Overview                                                    |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       2       |      12       |      22       |      102      |      112      |      122      |   By Year                                                     |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       3       |      13       |      23       |      103      |      113      |      123      |   By Month                                                    |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       4       |      14       |      24       |      104      |      114      |      124      |   By Station                                                  |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       5       |      15       |      25       |      105      |      115      |      125      |   By Year All Station                                         |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       6       |      16       |      26       |      106      |      116      |      126      |   By Month All Station                                        |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       7       |      17       |      27       |      107      |      117      |      127      |   By Station All Year                                         |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       8       |      18       |      28       |      108      |      118      |      128      |   By Station All Month                                        |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|---------------|---------------|---------------|---------------|---------------|---------------|---------------------------------------------------------------|");
    // println("|   Selection   |     Merge     |    Bubble     |   Selection   |    Bitonic    |    Odd Even   |   Histograms                                                  |");
    // println("|---------------|---------------|---------------|---------------|---------------|---------------|---------------------------------------------------------------|");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       51      |      61       |      71       |      151      |      161      |      171      |   Overview                                                    |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       52      |      62       |      72       |      152      |      162      |      172      |   By Year                                                     |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       53      |      63       |      73       |      153      |      163      |      173      |   By Month                                                    |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       54      |      64       |      74       |      154      |      164      |      174      |   By Station                                                  |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       55      |      65       |      75       |      155      |      165      |      175      |   By Year All Station                                         |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       56      |      66       |      76       |      156      |      166      |      176      |   By Month All Station                                        |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       57      |      67       |      77       |      157      |      167      |      177      |   By Station All Year                                         |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|       58      |      68       |      78       |      158      |      168      |      178      |   By Station All Month                                        |");
    // println("|               |               |               |               |               |               |                                                               |");
    // println("|_______________|_______________|_______________|_______________|_______________|_______________|_______________________________________________________________|");
    println("|            Serial Operations (xx)             |           Parallel Operations (xxx)           |   Description                                                 |");
    println("|-----------------------------------------------|-----------------------------------------------|---------------------------------------------------------------|");
    println("|   Selection   |     Merge     |    Bubble     |   Selection   |                               |   Summary                                                     |");
    println("|---------------|---------------|---------------|---------------|-------------------------------|---------------------------------------------------------------|");
    println("|               |               |               |               |                               |                                                               |");
    println("|       1       |      11       |      21       |      101      |                               |   Overview                                                    |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       2       |      12       |      22       |      102      |                               |   By Year                                                     |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       3       |      13       |      23       |      103      |                               |   By Month                                                    |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       4       |      14       |      24       |      104      |                               |   By Station                                                  |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       5       |      15       |      25       |      105      |                               |   By Year All Station                                         |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       6       |      16       |      26       |      106      |                               |   By Month All Station                                        |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       7       |      17       |      27       |      107      |                               |   By Station All Year                                         |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       8       |      18       |      28       |      108      |                               |   By Station All Month                                        |");
    println("|               |               |               |               |                               |                                                               |");
    println("|---------------|---------------|---------------|---------------|-------------------------------|---------------------------------------------------------------|");
    println("|   Selection   |     Merge     |    Bubble     |   Selection   |                               |   Histograms                                                  |");
    println("|---------------|---------------|---------------|---------------|-------------------------------|---------------------------------------------------------------|");
    println("|               |               |               |               |                               |                                                               |");
    println("|       51      |      61       |      71       |      151      |                               |   Overview                                                    |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       52      |      62       |      72       |      152      |                               |   By Year                                                     |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       53      |      63       |      73       |      153      |                               |   By Month                                                    |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       54      |      64       |      74       |      154      |                               |   By Station                                                  |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       55      |      65       |      75       |      155      |                               |   By Year All Station                                         |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       56      |      66       |      76       |      156      |                               |   By Month All Station                                        |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       57      |      67       |      77       |      157      |                               |   By Station All Year                                         |");
    println("|               |               |               |               |                               |                                                               |");
    println("|       58      |      68       |      78       |      158      |                               |   By Station All Month                                        |");
    println("|               |               |               |               |                               |                                                               |");
    println("|_______________|_______________|_______________|_______________|_______________________________|_______________________________________________________________|");
    println("|                                                                                                                                                               |");
    println("|                                                                      -1  |  Restart                                                                           |");
    println("|                                                                      -2  |  Set Dataset                                                                       |");
    println("|                                                                      -3  |  Set Bin Size                                                                      |");
    println("|                                                                      -4  |  Set Histogram Local Size                                                          |");
    println("|                                                                      -5  |  Set Sorting Local Size                                                            |");
    println("|                                                                     -10  |  Exit Program                                                                      |");
    println("|_______________________________________________________________________________________________________________________________________________________________|");
    println("");
}

void displayInfo_Operation(int operation)
{
    print("=================================================================================================================================================================");
    print(" [ Option " + to_string(operation) + ": ");
    switch (operation)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        print("Serial Selection Sort Operation] - Summary ");
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        print("Serial Merge Sort Operation] - Summary ");
        break;
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
        print("Serial Bubble Sort Operation] - Summary ");
        break;
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
        print("Serial Selection Sort Operation] - Histogram ");
        break;
    case 61:
    case 62:
    case 63:
    case 64:
    case 65:
    case 66:
    case 67:
    case 68:
        print("Serial Merge Sort Operation] - Histogram ");
        break;
    case 71:
    case 72:
    case 73:
    case 74:
    case 75:
    case 76:
    case 77:
    case 78:
        print("Serial Bubble Sort Operation] - Histogram ");
        break;
    case 101:
    case 102:
    case 103:
    case 104:
    case 105:
    case 106:
    case 107:
    case 108:
        print("Parallel Selection Sort Operation] - Summary ");
        break;
    case 111:
    case 112:
    case 113:
    case 114:
    case 115:
    case 116:
    case 117:
    case 118:
        print("Parallel Bitonic Sort Operation] - Summary ");
        break;
    case 151:
    case 152:
    case 153:
    case 154:
    case 155:
    case 156:
    case 157:
    case 158:
        print("Parallel Selection SortOperation] - Histogram ");
        break;
    case 161:
    case 162:
    case 163:
    case 164:
    case 165:
    case 166:
    case 167:
    case 168:
        print("Parallel Bitonic SortOperation] - Histogram ");
        break;
    default:
        print("Invalid Option");
        break;
    }

    switch (operation)
    {
    case 1:
    case 11:
    case 21:
    case 51:
    case 61:
    case 71:
    case 101:
    case 111:
    case 151:
    case 161:
        println("Overview");
        break;
    case 2:
    case 12:
    case 22:
    case 52:
    case 62:
    case 72:
    case 102:
    case 112:
    case 152:
    case 162:
        println("By Year");
        break;
    case 3:
    case 13:
    case 23:
    case 53:
    case 63:
    case 73:
    case 103:
    case 113:
    case 153:
    case 163:
        println("By Month");
        break;
    case 4:
    case 14:
    case 24:
    case 54:
    case 64:
    case 74:
    case 104:
    case 114:
    case 154:
    case 164:
        println("By Station");
        break;
    case 5:
    case 15:
    case 25:
    case 55:
    case 65:
    case 75:
    case 105:
    case 115:
    case 155:
    case 165:
        println("By Year All Station");
        break;
    case 6:
    case 16:
    case 26:
    case 56:
    case 66:
    case 76:
    case 106:
    case 116:
    case 156:
    case 166:
        println("By Month All Station");
        break;
    case 7:
    case 17:
    case 27:
    case 57:
    case 67:
    case 77:
    case 107:
    case 117:
    case 157:
    case 167:
        println("By Station All Year");
        break;
    case 8:
    case 18:
    case 28:
    case 58:
    case 68:
    case 78:
    case 108:
    case 118:
    case 158:
    case 168:
        println("By Station All Month");
        break;
    default:
        println("Invalid Option");
        break;
    }
    println();
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Data Summary
void displayInfo_Footer(clock_t startTime, clock_t endTime)
{
    cout << "|" << internal << setfill('-') << setw(160) << "|" << endl; // Padding end
    cout << "|" << internal << setfill(' ') << setw(160) << "|" << endl;
    cout << "| Total time taken (ms): " << left << setw(135) << setfill(' ') << long(endTime - startTime) << "|" << endl;
    cout << "|" << internal << setfill(' ') << setw(160) << "|" << endl;
    println("|_______________________________________________________________________________________________________________________________________________________________| END");
    println("");
    println("");
    println("");
}

void displayInfo_Summary(int size, float mean, float sDeviation, float min, float max, float median, float Q1, float Q3, clock_t startTime, clock_t endTime, string message = "")
{
    cout << left << setprecision(4) << fixed << setfill(' ')
         << "| "
         << setw(14) << min << "| "
         << setw(14) << max << "| "
         << setw(14) << mean << "| "
         << setw(14) << sDeviation << "| "
         << setw(14) << median << "| "
         << setw(14) << Q1 << "| "
         << setw(14) << Q3 << "| "
         << setw(14) << size << "| "
         << setw(14) << long(endTime - startTime) << "|";
    message == "" ? cout << endl : cout << " Note:" << message << endl;
}

void displayInfo_Histogram_Summary(float binStart, float binEnd, int frequency)
{
    cout << left << setfill(' ')
         << "| " << setw(14) << binStart
         << "| " << setw(14) << binEnd
         << "| " << setw(14) << frequency
         << "|" << internal << setfill(' ') << setw(112) << "|" << endl;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Headers

void displayInfo_Overall_Header()
{
    println("=================================================================================================================================================================");
    println("|                                                                                                                                                               |");
    println("|                                                                      [Overall Summary]                                                                        |");
    println("|                                                                                                                                                               |");
    println("=================================================================================================================================================================");
    println("|               |      Min      |      Max      |     Mean      | Std.Deviation |    Median     | 1st Quartile  | 3rd Quartile  |     Size      |   Time(ms)    |");
    println("|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|");
}

void displayInfo_By_Year_Header()
{
    println("=================================================================================================================================================================");
    println("|                                                                                                                                                               |");
    println("|                                                                      [By Month Summary]                                                                       |");
    println("|                                                                                                                                                               |");
    println("=================================================================================================================================================================");
    println("|     Year      |      Min      |      Max      |     Mean      | Std.Deviation |    Median     | 1st Quartile  | 3rd Quartile  |     Size      |   Time(ms)    |");
    println("|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|");
}

void displayInfo_By_Month_Header()
{
    println("=================================================================================================================================================================");
    println("|                                                                                                                                                               |");
    println("|                                                                      [By Month Summary]                                                                       |");
    println("|                                                                                                                                                               |");
    println("=================================================================================================================================================================");
    println("|     Month     |      Min      |      Max      |     Mean      | Std.Deviation |    Median     | 1st Quartile  | 3rd Quartile  |     Size      |   Time(ms)    |");
    println("|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|");
}

void displayInfo_By_Station_Header()
{
    println("=================================================================================================================================================================");
    println("|                                                                                                                                                               |");
    println("|                                                                     [By Station Summary]                                                                      |");
    println("|                                                                                                                                                               |");
    println("=================================================================================================================================================================");
    println("|    Station    |      Min      |      Max      |     Mean      | Std.Deviation |    Median     | 1st Quartile  | 3rd Quartile  |     Size      |   Time(ms)    |");
    println("|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|");
}

void displayInfo_Histogram_Header(int binNum, float binSize, float min, float max, unsigned long executionTime = 0)
{
    println("=================================================================================================================================================================");
    println("|                                                                                                                                                               |");
    println("|                                                                     [Histogram Summary]                                                                       |");
    println("|                                                                                                                                                               |");
    println("=================================================================================================================================================================");
    if (executionTime != 0)
    {
        println("|      Min      |      Max      |   Bin Number  |   Bin Size    | Ex.Time (ns)  |                                                                               |");
        println("|---------------|---------------|---------------|---------------|---------------|-------------------------------------------------------------------------------|");
        cout << left << setfill(' ')
             << "| " << setw(14) << min
             << "| " << setw(14) << max
             << "| " << setw(14) << binNum
             << "| " << setw(14) << binSize
             << "| " << setw(14) << executionTime
             << "| " << internal << setfill(' ') << setw(79) << "|" << endl;
    }
    else
    {
        println("|      Min      |      Max      |   Bin Number  |   Bin Size    |                                                                                               |");
        println("|---------------|---------------|---------------|---------------|-----------------------------------------------------------------------------------------------|");
        cout << left << setfill(' ')
             << "| " << setw(14) << min
             << "| " << setw(14) << max
             << "| " << setw(14) << binNum
             << "| " << setw(14) << binSize
             << "| " << internal << setfill(' ') << setw(95) << "|" << endl;
    }
    println("|---------------------------------------------------------------------------------------------------------------------------------------------------------------|");
    println("|                                                                                                                                                               |");
    println("|---------------------------------------------------------------------------------------------------------------------------------------------------------------|");
    println("| Bin Range Low | Bin Range Up  |   Frequency   |                                                                                                               |");
    println("|---------------|---------------|---------------|---------------------------------------------------------------------------------------------------------------|");
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Table Divider
void displayInfo_ByX_Header(string string1, string string2 = "", string string3 = "")
{
    if (string2 == "")
        cout << internal << setfill('=') << setw(162) << " " << string1 << endl;
    else if (string3 == "")
        cout << internal << setfill('=') << setw(162) << " " << string1 << " - " << string2 << endl;
    else
        cout << internal << setfill('=') << setw(162) << " " << string1 << " - " << string2 << " - " << string3 << endl;
}

void displayInfo_TableDiv(char c)
{
    cout << "|" << internal << setfill(c) << setw(160) << "|" << endl; // Padding// Process the last station's data
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Platform and Device Selection
void displayMenu_PlatformAndDeviceSelection(string list)
{
    println("++++++++++++++++++++++++++++++++++++++++++++++++++ Platform and Device Selection");
    println(list);
    println("++++++++++++++++++++++++++++++++++++++++++++++++++");
    println("");
}

void displayMenu_DatasetSelection()
{
    println("++++++++++++++++++++++++++++++++++++++++++++++++++ Dataset Selection");
    println("");
    println("   0. Debug Dataset");
    println("");
    println("   1. China Temperature Short Dataset");
    println("");
    println("   2. China Temperature Large Dataset");
    println("");
    println("++++++++++++++++++++++++++++++++++++++++++++++++++");
    println("");
}

void displayMenu_HistogramBinSizeSetting()
{
    println("++++++++++++++++++++++++++++++++++++++++++++++++++ Histogram Bin Size Setting");
    println("");
    println("   Please enter a bin size value between 1 and 1000");
    println("");
    println("++++++++++++++++++++++++++++++++++++++++++++++++++");
    println("");
}

void displayMenu_HistLocalSizeSetting()
{
    println("++++++++++++++++++++++++++++++++++++++++++++++++++ Histogram Local Size Setting");
    println("");
    println("   Please enter a local size value between 1 and 1024.");
    println("");
    println("   *** Make sure it is a power of 2 and less than or equal to the device's maximum work group size.");
    println("");
    println("++++++++++++++++++++++++++++++++++++++++++++++++++");
    println("");
}

void displayMenu_SortingLocalSizeSetting()
{
    println("++++++++++++++++++++++++++++++++++++++++++++++++++ Sorting Local Size Setting");
    println("");
    println("   Please enter a local size value between 1 and 1024.");
    println("");
    println("   *** Make sure it is a power of 2 and less than or equal to the device's maximum work group size.");
    println("");
    println("++++++++++++++++++++++++++++++++++++++++++++++++++");
    println("");
}

void displayInfo_ProgramConfiguration(string platformName, string deviceName, int binSize, int histLocalSize, int sortLocalSize)
{
    println("================================================== Platform and Device Information");
    println("Platform\t: " + platformName);
    println("Device\t\t: " + deviceName);
    println("================================================== Histogram Bin Size");
    println("Bin Size\t: " + to_string(binSize));
    println("================================================== Parallel Execution Local Size");
    println("Histogram Local Size\t: " + to_string(histLocalSize));
    println("Sorting Local Size\t: " + to_string(sortLocalSize));
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Utilities
pair<int, int> getPlatformAndDeviceSelection()
{
    int platform_id, device_id;
    print("Enter Platform ID | Device ID (e.g., '0 0' or '0 1'): ");
    cin >> platform_id >> device_id; // Take in 2 inputs for platform and device
    return make_pair(platform_id, device_id);
}

int getInput()
{
    int mainMenuChoice;
    print("Enter option: ");
    cin >> mainMenuChoice;
    return mainMenuChoice; // Return input choice
}

void pause()
{
    system("pause"); // Pause the console, press any key to continue
}

void clearScreen()
{
    system("cls"); // Clear screen
}

void refreshHeader(string platform_id, string device_name, int binSize, int histLocalSize, int sortLocalSize)
{
    clearScreen();                                                                                     // Clear the screen
    displayInfo_ProgramConfiguration(platform_id, device_name, binSize, histLocalSize, sortLocalSize); // Display the program configuration
    displayHeader();                                                                                   // Display Header with name and version etc
}

#endif // DISPLAY_H
