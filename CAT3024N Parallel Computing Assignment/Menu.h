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
    println("==================================================");
    println("     CAT3024N Parallel Computing - Assignment     ");
    println("==================================================");
    println("");
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Main Menu
void displayMenu_Main()
{
    println("++++++++++++++++++++++++++++++++++++++++++++++++++ Main Menu");
    println("SERIAL mode: ");
    println("1. Statistic Summary");
    println("2. Histogram Summary");
    println("");
    println("PARALLEL mode: ");
    println("3. Statistic Summary");
    println("4. Histogram Summary");
    println("");
    println("5. Quit Program");
    println("++++++++++++++++++++++++++++++++++++++++++++++++++");
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
