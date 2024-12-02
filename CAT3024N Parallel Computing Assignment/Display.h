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
    println("|                                                                            Summary                                                                            |");
    println("|------------------------------|------------------------------|-------------------------------------------------------------------------------------------------|");
    println("|                              |                              |                                                                                                 |");
    println("|                1             |             101              |   Overview                                                                                      |");
    println("|                              |                              |                                                                                                 |");
    println("|                2             |             102              |   By Year                                                                                       |");
    println("|                              |                              |                                                                                                 |");
    println("|                3             |             103              |   By Month                                                                                      |");
    println("|                              |                              |                                                                                                 |");
    println("|                4             |             104              |   By Station                                                                                    |");
    println("|                              |                              |                                                                                                 |");
    println("|                5             |             105              |   By Year All Station                                                                           |");
    println("|                              |                              |                                                                                                 |");
    println("|                6             |             106              |   By Month All Station                                                                          |");
    println("|                              |                              |                                                                                                 |");
    println("|                7             |             107              |   By Station All Year                                                                           |");
    println("|                              |                              |                                                                                                 |");
    println("|                8             |             108              |   By Station All Month                                                                          |");
    println("|                              |                              |                                                                                                 |");
    println("|                9             |             109              |   Full                                                                                          |");
    println("|                              |                              |                                                                                                 |");
    println("|------------------------------|------------------------------|-------------------------------------------------------------------------------------------------|");
    println("|                                                                           Histogram                                                                           |");
    println("|------------------------------|------------------------------|-------------------------------------------------------------------------------------------------|");
    println("|                              |                              |                                                                                                 |");
    println("|               51             |             151              |   Overview                                                                                      |");
    println("|                              |                              |                                                                                                 |");
    println("|               52             |             152              |   By Year                                                                                       |");
    println("|                              |                              |                                                                                                 |");
    println("|               53             |             153              |   By Month                                                                                      |");
    println("|                              |                              |                                                                                                 |");
    println("|               54             |             154              |   By Station                                                                                    |");
    println("|                              |                              |                                                                                                 |");
    println("|               55             |             155              |   By Year All Station                                                                           |");
    println("|                              |                              |                                                                                                 |");
    println("|               56             |             156              |   By Month All Station                                                                          |");
    println("|                              |                              |                                                                                                 |");
    println("|               56             |             156              |   By Station All Year                                                                           |");
    println("|                              |                              |                                                                                                 |");
    println("|               58             |             158              |   By Station All Month                                                                          |");
    println("|                              |                              |                                                                                                 |");
    println("|               59             |             159              |   Full                                                                                          |");
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
    case 9:
        print("Serial Operation] - Summary ");
        break;
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
        print("Serial Operation] - Histogram ");
        break;
    case 101:
    case 102:
    case 103:
    case 104:
    case 105:
    case 106:
    case 107:
    case 108:
    case 109:
        print("Parallel Operation] - Summary ");
        break;
    case 151:
    case 152:
    case 153:
    case 154:
    case 155:
    case 156:
    case 157:
    case 158:
        print("Parallel Operation] - Histogram ");
        break;
    default:
        println("Invalid Option");
        break;
    }
    switch (operation)
    {
    case 1:
    case 51:
    case 101:
    case 151:
        println("Overview");
        break;
    case 2:
    case 52:
    case 102:
    case 152:
        println("By Year");
        break;
    case 3:
    case 53:
    case 103:
    case 153:
        println("By Month");
        break;
    case 4:
    case 54:
    case 104:
    case 154:
        println("By Station");
        break;
    case 5:
    case 55:
    case 105:
    case 155:
        println("By Year All Station");
        break;
    case 6:
    case 56:
    case 106:
    case 156:
        println("By Month All Station");
        break;
    case 7:
    case 57:
    case 107:
    case 157:
        println("By Station All Year");
        break;
    case 8:
    case 58:
    case 108:
    case 158:
        println("By Station All Month");
        break;
    case 9:
    case 59:
    case 109:
    case 159:
        println("Full");
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
