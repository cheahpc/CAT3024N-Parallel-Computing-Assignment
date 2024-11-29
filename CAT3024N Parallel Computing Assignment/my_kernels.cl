// Atomic add function to work with floats
inline void atomicAddFloat(volatile __global float *addr, float val)
{
	// Create a union object between uint and float
	union
	{
		unsigned int u32;	   // Unsigned integer
		float f32;			   // Float
	} next, expected, current; // Define three variables based on the defined union
	// Set to addr
	current.f32 = *addr;
	// While current does not equal the expected do the following
	do
	{
		// Set expected to current
		expected.f32 = current.f32;
		// Set next to the expected + val
		next.f32 = expected.f32 + val;
		// Perform atomic cmpxchg with addr, expected and next
		current.u32 = atomic_cmpxchg((volatile __global unsigned int *)addr,
									 expected.u32, next.u32);
	} while (current.u32 != expected.u32);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Selection sort kernel
__kernel void p_SelectionSortAsc(__global const float *A, __global float *B)
{
	// Get global ID, global size
	int i = get_global_id(0);
	int n = get_global_size(0);

	// Set iKey to the current value
	float iKey = A[i];

	int pos = 0; // Position initialized to 0
	// Loop through the size
	for (int j = 0; j < n; j++)
	{
		// Get current value at j
		float jKey = A[j];
		// Get the smaller out of the two values
		bool smaller = (jKey < iKey) || (jKey == iKey && j < i);
		// Position + the result of smaller
		pos += (smaller) ? 1 : 0;
	}
	// Put variable into output
	B[pos] = A[i];
}

// Selection sort kernel descending
__kernel void p_SelectionSortDesc(__global const float *A, __global float *B)
{
	// Get global ID, global size
	int i = get_global_id(0);
	int n = get_global_size(0);

	// Set iKey to the current value
	float iKey = A[i];

	int pos = 0; // Position initialized to 0
	for (int j = 0; j < n; j++)
	{
		// Get current value at j
		float jKey = A[j];
		// Get the larger out of the two values
		bool larger = (jKey > iKey) || (jKey == iKey && j < i);
		// Position + the result of larger
		pos += (larger) ? 1 : 0;
	}
	// Put variable into output
	B[pos] = A[i];
}

// Merge sort kernel
// Merge function to merge two sorted subarrays
__kernel void merge(__global float *A, __global float *B, int width, int size)
{
	int global_id = get_global_id(0);
	int start = 2 * global_id * width;
	int middle = min(start + width, size);
	int end = min(start + 2 * width, size);

	int i = start;
	int j = middle;
	int k = start;

	while (i < middle && j < end)
	{
		if (A[i] < A[j])
		{
			B[k++] = A[i++];
		}
		else
		{
			B[k++] = A[j++];
		}
	}

	while (i < middle)
	{
		B[k++] = A[i++];
	}

	while (j < end)
	{
		B[k++] = A[j++];
	}
}

// Merge sort kernel
__kernel void p_MergeSort(__global float *A, __global float *B)
{
	int size = get_global_size(0);
	int width = 1; // Corrected initialization
	while (width < size)
	{
		int num_merges = (size + 2 * width - 1) / (2 * width);
		for (int i = 0; i < num_merges; i++)
		{
			merge(A, B, width, size);
		}
		width *= 2;
		// Synchronize all work-items before swapping
		barrier(CLK_GLOBAL_MEM_FENCE);
		// Swap A and B
		__global float *temp = A;
		A = B;
		B = temp;
		// Synchronize all work-items after swapping
		barrier(CLK_GLOBAL_MEM_FENCE);
	}
	// Ensure the final sorted array is in A
	if (width / 2 % 2 == 1)
	{
		for (int i = 0; i < size; i++)
		{
			B[i] = A[i];
		}
	}
}

// Reduce add kernel
__kernel void p_Sum(__global const float *A, __global float *B, __local float *scratch)
{
	// Get the global ID, local ID and local size
	int id = get_global_id(0);
	int lid = get_local_id(0);
	int N = get_local_size(0);

	// Copy global memory to local memory
	scratch[lid] = A[id];
	barrier(CLK_LOCAL_MEM_FENCE);

	// Loop through local memory
	for (int i = 1; i < N; i *= 2)
	{
		// Check if to operate, if yes then add the current local value to the next
		if (!(lid % (i * 2)) && ((lid + i) < N))
			scratch[lid] += scratch[lid + i];
		// Wait for all local threads to finish copying from global to local memory
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	// Add all of the values from local back to global position 0
	if (!lid)
		atomicAddFloat(&B[0], scratch[lid]);
}

// Reduce standard deviation kernel
__kernel void p_Standard_Deviation(__global const float *A, __global float *B, __local float *scratch, float mean, int padd)
{
	// Get global ID, Local ID, Local size, Global size, Size
	int id = get_global_id(0);
	int lid = get_local_id(0);
	int n = get_local_size(0);
	int ng = get_global_size(0);
	int size = ng - padd;

	// Cache all N values from global memory to local memory, that are less than NG-Padd
	if (id < (ng - padd))
	{
		scratch[lid] = A[id];
		scratch[lid] = ((scratch[lid] - mean) * (scratch[lid] - mean)); // Perform first operation for standard deviation
	}
	// Wait for all local threads to finish copying from global to local memory
	barrier(CLK_LOCAL_MEM_FENCE);
	// Loop through all local memory
	for (int i = 1; i < n; i *= 2)
	{
		// Check if to operate, if yes then add the current local value to the next
		if (!(lid % (i * 2)) && ((lid + i) < n))
			scratch[lid] += scratch[lid + i];
		// Wait for all local threads to finish copying from global to local memory
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	// Add all of the values from local back to global position 0
	if (!lid)
		atomicAddFloat(&B[0], scratch[lid]);
}

// Histogram kernel
kernel void hist_simple(global const float *temperature, global int *output, int bincount, float minval, float maxval)
{
	int id = get_global_id(0);
	int n = 0;
	// get range of values it can be
	float range = maxval - minval;
	float i = temperature[id];

	float increment = range / bincount; // each increment is range / the number of bins wanted

	float compareval = minval + increment; // initial increment
	while (i > compareval)
	{
		compareval += increment; // get next increment
		n++;
	}
	if (n == bincount)
		n = bincount - 1;

	if (i <= maxval)
		atomic_inc(&output[n]);
}