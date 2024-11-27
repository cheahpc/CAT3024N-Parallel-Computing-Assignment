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
    println("      1. Overall Summary");
    println("      2. By Month Summary");
    println("      3. By Station Summary");
    println("      4. Complete Summary (by Station and Month)");
    println("      5. Histogram Summary - Serial Operation");
    println("");
    println("Parallel Operation Option:");
    println("      6. Overall Summary");
    println("      7. By Month Summary");
    println("      8. By Station Summary");
    println("      9. Complete Summary (by Station and Month)");
    println("      10. Histogram Summary - Serial Operation");
    println("");
    println("      11. Quit Program");
    println("");
    println("++++++++++++++++++++++++++++++++++++++++++++++++++");
}

void displayHeader_Operation(int operation)
{
    println();
    switch (operation)
    {
    case 1:
        println("================================================== [Option 1 - Serial] Overall Summary");
        break;
    case 2:
        println("================================================== [Option 2 - Serial] By Month Summary");
        break;

    default:
        break;
    }
    println();
    println();
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Data Summary
void displayInfo_Overall(int size, float mean, float sDeviation, float min, float max, float median, float Q1, float Q3, float startTime, float endTime)
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
    println("|-----------------------------------------------------------------------------------------------------");
    printf("|               Size: %d\n", size);
    printf("|               Time: %d ms\n", (endTime - startTime));
    println("|_____________________________________________________________________________________________________");
    println();
    println("================================================[END]================================================");
}

void displayInfo_By_Month(bool header = true, )
{
    // Header
    println("==============================================[By Month]===============================================");
    println("| Month | Min \t| Max \t| Mean \t| SD \t| Median \t| 1Q \t| 3Q \t| Size \t| Time\t|");
    println("|-------|--------|-------|-------|-------|-----------|-------|-------|-------|-------|"); 

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
