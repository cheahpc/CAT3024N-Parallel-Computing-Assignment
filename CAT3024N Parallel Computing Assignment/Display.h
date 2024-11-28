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

void displayInfo_Student()
{
    println("================================================== Student Information");
    println("Student Name\t: Cheah Pin Chee");
    println("Student ID\t: 0197637");
    println();
}

void displayHeader()
{
    println("====================================================================================================");
    println("|                             CAT3024N Parallel Computing - Assignment                             |");
    println("====================================================================================================");
    println();
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Main Menu
void displayMenu_Main()
{
    println("++++++++++++++++++++++++++++++++++++++++++++++++++ Main Menu");
    println("");
    println("      0. Display Stations Name");
    println("");
    println("Serial Operation Option:");
    println("");
    println("      1. Overall Summary");
    println("      2. By Month Summary");
    println("      3. By Station Summary");
    println("      4. By Month All Station Summary");
    println("      5. By Station All Month Summary");
    println("      6. Full Summary");
    println("      7. Histogram Summary");
    println("");
    println("Parallel Operation Option:");
    println("");
    println("      8. Overall Summary");
    println("      9. By Month Summary");
    println("     10. By Station Summary");
    println("     11. By Month All Station Summary");
    println("     12. By Station All Month Summary");
    println("     13. Full Summary");
    println("     14. Histogram Summary");
    println("");
    println("      15. Quit Program");
    println("");
    println("++++++++++++++++++++++++++++++++++++++++++++++++++");
}

void displayInfo_Operation(int operation)
{
    println();
    switch (operation)
    {
    case 1:
        println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ [Option 1 - Serial] Overall Summary ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        break;
    case 2:
        println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ [Option 2 - Serial] By Month Summary ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        break;

    default:
        break;
    }
    println();
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
}

void displayInfo_Summary(int size, float mean, float sDeviation, float min, float max, float median, float Q1, float Q3, clock_t startTime, clock_t endTime, string message = "")
{

    cout << left << setprecision(2) << fixed << setfill(' ')
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
    println("|      Min      |      Max      |     Mean      | Std.Deviation |    Median     | 1st Quartile  | 3rd Quartile  |     Size      |   Time(ms)    |               |");
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

void displayInfo_Histogram_Header(int binNum, float binSize, float min, float max)
{
    println("=================================================================================================================================================================");
    println("|                                                                                                                                                               |");
    println("|                                                                     [Histogram Summary]                                                                       |");
    println("|                                                                                                                                                               |");
    println("=================================================================================================================================================================");
    println("|      Min      |      Max      |   Bin Number  |   Bin Size    |                                                                                               |");
    println("|---------------|---------------|---------------|---------------|-----------------------------------------------------------------------------------------------|");
    cout << left << setfill(' ')
         << "| " << setw(14) << min
         << "| " << setw(14) << max
         << "| " << setw(14) << binNum
         << "| " << setw(14) << binSize
         << "| " << internal << setfill(' ') << setw(95) << "|" << endl;
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
    displayInfo_Student();
    displayHeader();
}

#endif // DISPLAY_H
