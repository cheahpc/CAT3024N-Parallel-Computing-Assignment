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
const string SERIAL_HISTOGRAM_CSV =   "Histogram_Serial.csv";
const string PARALLEL_HISTOGRAM_CSV = "Histogram_Parallel.csv";

// Vectors

// Sorting Enum
typedef enum SORT_ORDER
{
    ASCENDING,
    DESCENDING
};
#endif // GLOBAL_H