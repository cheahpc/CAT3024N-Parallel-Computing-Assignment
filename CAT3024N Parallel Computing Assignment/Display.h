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
    println("|     Serial Operations (x)    |  Parallel Operations (1xx)   |   Description                                                                                   |");
    println("|------------------------------|------------------------------|-------------------------------------------------------------------------------------------------|");
    println("|                              |                              |                                                                                                 |");
    println("|                1             |             101              |   Overall Summary                                                                               |");
    println("|                              |                              |                                                                                                 |");
    println("|                2             |             102              |   By Year                                                                                       |");
    println("|                              |                              |                                                                                                 |");
    println("|                3             |             103              |   By Month Summary                                                                              |");
    println("|                              |                              |                                                                                                 |");
    println("|                4             |             104              |   By Station Summary                                                                            |");
    println("|                              |                              |                                                                                                 |");
    println("|                5             |             105              |   By Month All Station Summary                                                                  |");
    println("|                              |                              |                                                                                                 |");
    println("|                6             |             106              |   By Station All Month Summary                                                                  |");
    println("|                              |                              |                                                                                                 |");
    println("|                7             |             107              |   Full Summary                                                                                  |");
    println("|                              |                              |                                                                                                 |");
    println("|                8             |             108              |   Histogram Overall                                                                             |");
    println("|                              |                              |                                                                                                 |");
    println("|                9             |             109              |   Histogram Year                                                                                |");
    println("|                              |                              |                                                                                                 |");
    println("|               10             |             110              |   Histogram By Month                                                                            |");
    println("|                              |                              |                                                                                                 |");
    println("|               11             |             111              |   Histogram By Station                                                                          |");
    println("|                              |                              |                                                                                                 |");
    println("|               12             |             112              |   Histogram By Month All Station                                                                |");
    println("|                              |                              |                                                                                                 |");
    println("|               13             |             113              |   Histogram By Station All Month                                                                |");
    println("|                              |                              |                                                                                                 |");
    println("|______________________________|______________________________|_________________________________________________________________________________________________|");
    println("|                                                                                                                                                               |");
    println("|                                                                      -1  |  Exit Program                                                                      |");
    println("|_______________________________________________________________________________________________________________________________________________________________|");
    println("");
}

void displayInfo_Operation(int operation)
{
    print("=================================================================================================================================================================");
    print(" [ Option " + to_string(operation) + " ] - ");
    switch (operation)
    {
    case 1:
        print("Overall Summary");
        break;
    case 2:
        print("By Year Summary");
        break;
    case 3:
        print("By Month Summary");
        break;
    case 4:
        print("By Station Summary");
        break;
    case 5:
        print("By Month All Station Summary");
        break;
    case 6:
        print("By Station All Month Summary");
        break;
    case 7:
        print("Full Summary");
        break;
    case 8:
        print("Histogram Overall");
        break;
    case 9:
        print("Histogram By Year");
        break;
    case 10:
        print("Histogram By Month");
        break;
    case 11:
        print("Histogram By Station");
        break;
    case 12:
        print("Histogram By Month All Station");
        break;
    case 13:
        print("Histogram By Station All Month");
        break;
    case 101:
        print("Parallel Overall Summary");
        break;
    case 102:
        print("Parallel By Year Summary");
        break;
    case 103:
        print("Parallel By Month Summary");
        break;
    case 104:
        print("Parallel By Station Summary");
        break;
    case 105:
        print("Parallel By Month All Station Summary");
        break;
    case 106:
        print("Parallel By Station All Month Summary");
        break;
    case 107:
        print("Parallel Full Summary");
        break;
    case 108:
        print("Parallel Histogram Overall");
        break;
    case 109:
        print("Parallel Histogram By Year");
        break;
    case 110:
        print("Parallel Histogram By Month");
        break;
    case 111:
        print("Parallel Histogram By Station");
        break;
    case 112:
        print("Parallel Histogram By Month All Station");
        break;
    case 113:
        print("Parallel Histogram By Station All Month");
        break;
    default:
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
    // TODO cumulative frequency?
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

void displayInfo_PlatformAndDevice(string platformName, string deviceName)
{
    println("================================================== Platform and Device Information");
    println("Platform\t: " + platformName);
    println("Device\t\t: " + deviceName);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Utilities
pair<int, int> getPlatformAndDeviceSelection()
{
    int platform_id, device_id;
    print("Enter Platform ID | Device ID (e.g., '0 0' or '0 1'): ");
    cin >> platform_id >> device_id; // Take in 2 inputs for platform and device
    return make_pair(platform_id, device_id);
}

int getMenuChoice()
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

void refreshHeader(string platform_id, string device_name)
{
    clearScreen();                                           // Clear the screen
    displayInfo_PlatformAndDevice(platform_id, device_name); // Display Platform and Device Information
    displayHeader();                                         // Display Header with name and version etc
}

#endif // DISPLAY_H
