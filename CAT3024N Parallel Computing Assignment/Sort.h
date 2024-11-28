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

void merge(vector<int> &arr, vector<int> &indices, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);
    vector<int> L_indices(n1), R_indices(n2);

    for (int i = 0; i < n1; ++i)
    {
        L[i] = arr[left + i];
        L_indices[i] = indices[left + i];
    }

    for (int j = 0; j < n2; ++j)
    {
        R[j] = arr[mid + 1 + j];
        R_indices[j] = indices[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            indices[k] = L_indices[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            indices[k] = R_indices[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        indices[k] = L_indices[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        indices[k] = R_indices[j];
        j++;
        k++;
    }
}

void mergeSortHelper(vector<int> &arr, vector<int> &indices, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, indices, left, mid);
        mergeSortHelper(arr, indices, mid + 1, right);
        merge(arr, indices, left, mid, right);
    }
}

vector<int> mergeSortIndices(vector<int> &arr)
{
    int n = arr.size();
    vector<int> indices(n);
    for (int i = 0; i < n; ++i)
        indices[i] = i;

    mergeSortHelper(arr, indices, 0, n - 1);
    return indices;
}

#endif // SORT_H