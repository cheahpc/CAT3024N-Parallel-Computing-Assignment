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
    println("|     Serial Operations (x)    |   Parallel Operations (1x)   |   Description                                                                                   |");
    println("|------------------------------|------------------------------|-------------------------------------------------------------------------------------------------|");
    println("|                              |                              |                                                                                                 |");
    println("|               1              |             11               |   Overall Summary                                                                               |");
    println("|               2              |             12               |   By Month Summary                                                                              |");
    println("|               3              |             13               |   By Station Summary                                                                            |");
    println("|               4              |             14               |   By Month All Station Summary                                                                  |");
    println("|               5              |             15               |   By Station All Month Summary                                                                  |");
    println("|               6              |             16               |   Full Summary                                                                                  |");
    println("|               7              |             17               |   By Month Summary                                                                              |");
    println("|               8              |             18               |   -                                                                                             |");
    println("|______________________________|______________________________|_________________________________________________________________________________________________|");
    println("|                                                                                                                                                               |");
    println("|                                                                     100  |  Exit Program                                                                      |");
    println("|_______________________________________________________________________________________________________________________________________________________________|");
    println("");
}

void displayInfo_Operation(int operation)
{
    print("=================================================================================================================================================================");
    switch (operation)
    {
    case 1:
        print(" [ Option 1 - Serial Operation ] Overall Summary");
        break;
    case 2:
        print(" [ Option 2 - Serial Operation ] By Month Summary");
        break;
    case 3:
        print(" [ Option 3 - Serial Operation ] By Station Summary");
        break;
    case 4:
        print(" [ Option 4 - Serial Operation ] By Month All Station Summary");
        break;
    case 5:
        print(" [ Option 5 - Serial Operation ] By Station All Month Summary");
        break;
    case 6:
        print(" [ Option 6 - Serial Operation ] Full Summary");
        break;
    case 7:
        print(" [ Option 7 - Serial Operation ] By Month Summary");
        break;
    case 11:
        print(" [ Option 11 - Parallel Operation ] Overall Summary");
        break;
    case 12:
        print(" [ Option 12 - Parallel Operation ] By Month Summary");
        break;
    case 13:
        print(" [ Option 13 - Parallel Operation ] By Station Summary");
        break;
    case 14:
        print(" [ Option 14 - Parallel Operation ] By Month All Station Summary");
        break;
    case 15:
        print(" [ Option 15 - Parallel Operation ] By Station All Month Summary");
        break;
    case 16:
        print(" [ Option 16 - Parallel Operation ] Full Summary");
        break;
    case 17:
        print(" [ Option 17 - Parallel Operation ] By Month Summary");
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
             << "| " << internal << setfill(' ') << setw(95) << "|" << endl;
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
    println("|_______________________________________________________________________________________________________________________________________________________________|");
    println("|                                                                                                                                                               |");
    println("|                                                                                                                                                               |");
    println("|                                                                                                                                                               |");
    println("|---------------|---------------|---------------|---------------------------------------------------------------------------------------------------------------|");
    println("| Bin Range Low | Bin Range Up  |   Frequency   |                                                                                                               |");
    println("|---------------|---------------|---------------|---------------------------------------------------------------------------------------------------------------|");
    // TODO cumulative frequency?
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

pair<int, int> getPlatformAndDeviceSelection()
{
    int platform_id, device_id;
    print("Enter Platform ID | Device ID (e.g., '0 0' or '0 1'): ");
    cin >> platform_id >> device_id;
    return make_pair(platform_id, device_id);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Utilities
int getMenuChoice()
{
    int mainMenuChoice;
    print("Enter option: ");
    cin >> mainMenuChoice;
    return mainMenuChoice;
}

void pause()
{
    system("pause");
}

void clearScreen()
{
    system("cls");
}

void refreshHeader(string platform_id, string device_name)
{
    clearScreen();
    displayInfo_PlatformAndDevice(platform_id, device_name);
    displayHeader();
}

#endif // DISPLAY_H
