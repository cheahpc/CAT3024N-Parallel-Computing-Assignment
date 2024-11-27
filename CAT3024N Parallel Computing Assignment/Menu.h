#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>

using namespace std;

void print(string str)
{
    cout << str;
}

void println(string str)
{
    cout << str << endl;
}

void displayInfo_Student()
{
    println("================================================== Student Information");
    println("Student Name\t: Cheah Pin Chee");
    println("Student ID\t: 0197637");
    println("");
}

void displayHeader()
{
    println("====================================================================================================");
    println("|                             CAT3024N Parallel Computing - Assignment                             |");
    println("====================================================================================================");
    println("");
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Main Menu
void displayMenu_Main()
{
    println("++++++++++++++++++++++++++++++++++++++++++++++++++ Main Menu");
    println("Option.");
    println("      0. Display Stations Name");
    println("      1. Statistic Summary - Serial Operation");
    println("      2. Histogram Summary - Serial Operation");
    println("      3. Statistic Summary - - Parallel Operation");
    println("      4. Histogram Summary - - Parallel Operation");
    println("");
    println("      5. Quit Program");
    println("");
    println("++++++++++++++++++++++++++++++++++++++++++++++++++");
}

void displayHeader_Operation(int operation)
{
    println("");
    switch (operation)
    {
    case 1:
        println("================================================== [Option 1: Statistic Summary - Serial Operation]");
        break;
    case 2:
        println("================================================== [Option 2: Histogram Summary - Serial Operation]");
        break;
    case 3:
        println("================================================== [Option 3: Statistic Summary - Parallel Operation]");
        break;
    case 4:
        println("================================================== [Option 4: Histogram Summary - Parallel Operation]");
        break;

    default:
        break;
    }
    println("");
    println("");
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

#endif // MENU_H
