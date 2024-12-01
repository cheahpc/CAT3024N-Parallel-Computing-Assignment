#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
using namespace std;

// Constants
// const string DATASET_PATH = "china_temp_debug.txt"; // Debug
const string DATASET_PATH = "china_temp_short.txt"; // Development
// const string DATASET_PATH = "china_temp_large.txt"; // Final
const string KERNEL_PATH = "my_kernels.cl";

// Months
const string MONTH_LIST[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Histograms
const int HISTOGRAM_BIN_NO = 15;

// Sorting Enum
typedef enum SORT_ORDER
{
    ASCENDING,
    DESCENDING
};

// Switches, comment or uncomment to enable or disable
// #define SERIAL_PLOT  // Enable to plot the histogram using Python for serial
// #define PARALLEL_PLOT // Enable to plot the histogram using Python for parallel
#endif // GLOBAL_H