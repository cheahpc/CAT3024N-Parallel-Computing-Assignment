#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <iomanip>
#include <string>

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
    cout << left << setfill('_')
         << setw(16) << "|"
         << setw(16) << "|"
         << setw(16) << "|"
         << setw(16) << "|"
         << setw(16) << "|"
         << setw(16) << "|"
         << setw(16) << "|"
         << setw(16) << "|"
         << setw(16) << "|"
         << setw(16) << "|"
         << "|" << endl;
    println("|");
    println("| Time taken: " + to_string(long(endTime - startTime)) + " ms");
    println("|");
    println("_________________________________________________________________________________________________________________________________________________________________ END");
}

void displayInfo_Summary(bool isOverall, int size, float mean, float sDeviation, float min, float max, float median, float Q1, float Q3, clock_t startTime, clock_t endTime)
{
    if (isOverall)
    {
        println("===========================================[Overall Summary]===========================================");
        println("|");
        printf("|                Min: %.4f\n", min);
        printf("|                Max: %.4f\n", max);
        printf("|               Mean: %.4f\n", mean);
        printf("| Standard Deviation: %.4f\n", sDeviation);
        printf("|             Median: %.4f\n", median);
        printf("|       1st Quartile: %.4f\n", Q1);
        printf("|       3rd Quartile: %.4f\n", Q3);
        println("|");
        println("-------------------------------------------------------------------------------------------------------");
        println("|");
        printf("|               Size: %d\n", size);
        printf("|               Time: %ld ms\n", long(endTime - startTime));
        println("|");
        println("_______________________________________________________________________________________________________ END");
    }
    else
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
             << setw(14) << long(endTime - startTime) << "|"
             << endl;
    }
}

void displayInfo_By_Month_Header()
{
    println("=======================================================================[By Month Summary]========================================================================");
    println("|     Month     |      Min      |      Max      |     Mean      | Std.Deviation |    Median     | 1st Quartile  | 3rd Quartile  |     Size      |   Time(ms)    |");
    println("|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|");
}

void displayInfo_By_Station_Header()
{
    println("======================================================================[By Station Summary]=======================================================================");
    println("|    Station    |      Min      |      Max      |     Mean      | Std.Deviation |    Median     | 1st Quartile  | 3rd Quartile  |     Size      |   Time(ms)    |");
    println("|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|---------------|");
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
