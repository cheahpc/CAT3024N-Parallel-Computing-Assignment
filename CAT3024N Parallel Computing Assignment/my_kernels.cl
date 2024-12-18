// Helper functions: atomic add to float, swap
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

inline void swap(__global float *a, __global float *b)
{
	float temp = *a;
	*a = *b;
	*b = temp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parallel Odd Even Sort kernel
kernel void p_OddEvenSort(global const float *A, __global float *B)
{
	int id = get_global_id(0);	   // Get the thread ID
	int size = get_global_size(0); // Total number of threads

	// Copy input to output
	B[id] = A[id];
	barrier(CLK_GLOBAL_MEM_FENCE);

	for (int i = 0; i < size; i++)
	{
		// Even phase
		if (id % 2 == 0 && id + 1 < size)
		{
			if (B[id] > B[id + 1])
			{
				swap(&B[id], &B[id + 1]);
			}
		}
		barrier(CLK_GLOBAL_MEM_FENCE);

		// Odd phase
		if (id % 2 == 1 && id + 1 < size)
		{
			if (B[id] > B[id + 1])
			{
				swap(&B[id], &B[id + 1]);
			}
		}
		barrier(CLK_GLOBAL_MEM_FENCE);
	}
}

// Selection sort kernel
__kernel void p_SelectionSort(__global const float *A, __global float *B)
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

// Parallel merge sort (Bitonic) kernel
__kernel void p_BitonicSort(__global const float *A, __global float *B)
{
	// Get global ID, local ID, local size
	int id = get_global_id(0);
	int n = get_global_size(0);

	// Copy input to output
	B[id] = A[id];
	barrier(CLK_GLOBAL_MEM_FENCE);

	for (int size = 2; size <= n; size <<= 1)
	{
		for (int stride = size >> 1; stride > 0; stride >>= 1)
		{
			int pos = 2 * id - (id & (stride - 1));
			if (pos + stride < n)
			{
				if ((pos & size) == 0)
				{
					if (B[pos] > B[pos + stride])
					{
						swap(&B[pos], &B[pos + stride]);
					}
				}
				else
				{
					if (B[pos] < B[pos + stride])
					{
						swap(&B[pos], &B[pos + stride]);
					}
				}
			}
			barrier(CLK_LOCAL_MEM_FENCE);
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
kernel void p_Hist(global const float *temperature, global int *output, int bincount, float minval, float maxval)
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