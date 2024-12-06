#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
using namespace std;

// Kernel Path
const string KERNEL_PATH = "my_kernels.cl";

// Months
const string MONTH_LIST[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Sorting Enum
typedef enum SORT_ORDER
{
    ASCENDING,
    DESCENDING
};

// Switches, comment or uncomment to enable or disable
#define SERIAL_PLOT   // Enable to plot the histogram using Python for serial
#define PARALLEL_PLOT // Enable to plot the histogram using Python for parallel

// Sorting Algorithms
typedef enum SORT_ALGORITHM
{
    MERGE,
    SELECTION,
    BUBBLE
};

#endif // GLOBAL_H