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
    println("|                9             |             109              |   Histogram By Month                                                                            |");
    println("|                              |                              |                                                                                                 |");
    println("|               10             |             110              |   Histogram By Station                                                                          |");
    println("|                              |                              |                                                                                                 |");
    println("|               11             |             111              |   Histogram By Month All Station                                                                |");
    println("|                              |                              |                                                                                                 |");
    println("|               12             |             112              |   Histogram By Station All Month                                                                |");
    println("|______________________________|______________________________|_________________________________________________________________________________________________|");
    println("|                                                                                                                                                               |");
    println("|                                                                      -1  |  Exit Program                                                                      |");
    println("|_______________________________________________________________________________________________________________________________________________________________|");
    println("");
}

void displayInfo_Operation(int operation)
{
    print("=================================================================================================================================================================");
    switch (operation)
    {
    case 1:
        print(" [ Option 1 - Parallel Operation ] Overall Summary");
        break;
    case 2:
        print(" [ Option 2 - Parallel Operation ] By Year Summary");
        break;
    case 3:
        print(" [ Option 3 - Parallel Operation ] By Month Summary");
        break;
    case 4:
        print(" [ Option 4 - Parallel Operation ] By Station Summary");
        break;
    case 5:
        print(" [ Option 5 - Parallel Operation ] By Month All Station Summary");
        break;
    case 6:
        print(" [ Option 6 - Parallel Operation ] By Station All Month Summary");
        break;
    case 7:
        print(" [ Option 7 - Parallel Operation ] Full Summary");
        break;
    case 8:
        print(" [ Option 8 - Parallel Operation ] Histogram Overall");
        break;
    case 9:
        print(" [ Option 9 - Parallel Operation ] Histogram By Month");
        break;
    case 10:
        print(" [ Option 10 - Parallel Operation ] Histogram By Station");
        break;
    case 11:
        print(" [ Option 11 - Parallel Operation ] Histogram By Month All Station");
        break;
    case 12:
        print(" [ Option 12 - Parallel Operation ] Histogram By Station All Month");
        break;
    case 101:
        print(" [ Option 101 - Parallel Operation ] Overall Summary");
        break;
    case 102:
        print(" [ Option 102 - Parallel Operation ] By Year Summary");
        break;
    case 103:
        print(" [ Option 103 - Parallel Operation ] By Month Summary");
        break;
    case 104:
        print(" [ Option 104 - Parallel Operation ] By Station Summary");
        break;
    case 105:
        print(" [ Option 105 - Parallel Operation ] By Month All Station Summary");
        break;
    case 106:
        print(" [ Option 106 - Parallel Operation ] By Station All Month Summary");
        break;
    case 107:
        print(" [ Option 107 - Parallel Operation ] Full Summary");
        break;
    case 108:
        print(" [ Option 108 - Parallel Operation ] Histogram Overall");
        break;
    case 109:
        print(" [ Option 109 - Parallel Operation ] Histogram By Month");
        break;
    case 110:
        print(" [ Option 110 - Parallel Operation ] Histogram By Station");
        break;
    case 111:
        print(" [ Option 111 - Parallel Operation ] Histogram By Month All Station");
        break;
    case 112:
        print(" [ Option 112 - Parallel Operation ] Histogram By Station All Month");
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
    println("|_______________________________________________________________________________________________________________________________________________________________|");
    println("|                                                                                                                                                               |");
    println("|                                                                                                                                                               |");
    println("|                                                                                                                                                               |");
    println("|---------------|---------------|---------------|---------------------------------------------------------------------------------------------------------------|");
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
