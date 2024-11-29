import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Data format [start, end, frequency], no header

# Load histogram data from file
data = pd.read_csv('Parallel_Histogram.csv')

# Check if histogram data is empty
if data.empty:
    print('No data in Parallel_Histogram.csv')
    exit()

# Generate histogram data
data = data.to_numpy()
bins = data[:, :2]
frequencies = data[:, 2].astype(int)

data = []
for i, (start, end) in enumerate(bins):
    data.extend([np.random.uniform(start, end) for _ in range(frequencies[i])])

# Plot histogram
plt.hist(data, bins=len(bins), range=(bins[0][0], bins[-1][1]), edgecolor='black', color='red')

# Add value annotations to bars
for i, freq in enumerate(frequencies):
    plt.text((bins[i][0] + bins[i][1]) / 2, freq, str(freq), ha='center', va='bottom')

# Set labels and title
plt.xlabel('Temperature')
plt.ylabel('Frequency')
plt.title('Parallel Histogram For Weather Index')

# Save plot as image
plt.savefig('Parallel Histogram.png', dpi=1200)

# Show plot
plt.show()
