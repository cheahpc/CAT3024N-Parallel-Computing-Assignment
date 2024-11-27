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

void displayHeader()
{
    println("================================================== Student Information");
    println("Student Name\t: Cheah Pin Chee");
    println("Student ID\t: 0197637");
    println("==================================================");
    println("");

    println("==================================================");
    println("     CAT3024N Parallel Computing - Assignment     ");
    println("==================================================");
    println("");
}

void displayMainMenu()
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

#endif // MENU_H
