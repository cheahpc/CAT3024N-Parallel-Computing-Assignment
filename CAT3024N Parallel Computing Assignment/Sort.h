#ifndef SORT_H
#define SORT_H

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

// Function to perform selection sort on a vector of float elements
// This is intended to be used in serial execution
// This sorting algorithm processes the elements one after another, in sequence
void selectionSort(vector<float> &Values)
{
    int min_id_x;
    // One by one move boundary of unsorted array
    for (int i = 0; i < Values.size() - 1; i++)
    {
        min_id_x = i;
        for (int j = i + 1; j < Values.size(); j++)
        {
            // Store the index of minimum element in unsorted array
            if (Values[j] < Values[min_id_x])
            {
                min_id_x = j;
            }
        }
        // Swap the found min element with the bigger element
        if (min_id_x != i)
        {
            // Sorting or Swapping elements
            float temp = Values[min_id_x]; // Put the min value into temporary var
            Values[min_id_x] = Values[i];  // Put the bigger element to the min value's element array
            Values[i] = temp;              // Put the min element to go into the bigger element array
        }
    }
    return;
}

void merge(vector<float> &Values, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    vector<float> Left(n1), Right(n2);

    // Copy data to temporary arrays Left[] and Right[]
    for (int i = 0; i < n1; i++)
        Left[i] = Values[left + i];
    for (int j = 0; j < n2; j++)
        Right[j] = Values[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (Left[i] <= Right[j])
        {
            Values[k] = Left[i];
            i++;
        }
        else
        {
            Values[k] = Right[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of Left[], if there are any
    while (i < n1)
    {
        Values[k] = Left[i];
        i++;
        k++;
    }

    // Copy the remaining elements of Right[], if there are any
    while (j < n2)
    {
        Values[k] = Right[j];
        j++;
        k++;
    }
}

// Recursive function to implement merge sort
void mergeSort(vector<float> &Values, int left, int right)
{
    if (left < right)
    {
        // Find the middle point
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(Values, left, mid);
        mergeSort(Values, mid + 1, right);

        // Merge the sorted halves
        merge(Values, left, mid, right);
    }
}

// Function to call the mergeSort function
void mergeSort(vector<float> &Values)
{
    mergeSort(Values, 0, Values.size() - 1);
}

#endif // SORT_H