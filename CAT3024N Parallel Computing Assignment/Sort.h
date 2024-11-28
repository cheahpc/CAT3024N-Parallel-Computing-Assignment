#ifndef SORT_H
#define SORT_H

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

// Function to perform selection sort on a vector of float elements
void selectionSort(vector<float> &Values)
{
    int min_index;
    // One by one move boundary of unsorted array
    for (int i = 0; i < Values.size() - 1; i++)
    {
        min_index = i;
        for (int j = i + 1; j < Values.size(); j++)
        {
            // Store the index of minimum element in unsorted array
            if (Values[j] < Values[min_index])
            {
                min_index = j;
            }
        }
        // Swap the found min element with the bigger element
        if (min_index != i)
        {
            // Sorting or Swapping elements
            float temp = Values[min_index]; // Put the min value into temporary var
            Values[min_index] = Values[i];  // Put the bigger element to the min value's element array
            Values[i] = temp;               // Put the min element to go into the bigger element array
        }
    }
    return;
}



#endif // SORT_H