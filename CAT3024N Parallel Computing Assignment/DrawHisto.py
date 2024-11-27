import matplotlib.pyplot as plt
import numpy as np

# Load histogram data from file
with open('histogram.txt', 'r') as file:
    lines = file.readlines()

# Parse histogram data
bins = []
frequencies = []
for line in lines:
    start, end, freq = map(float, line.split())
    frequencies.append(int(freq))
    bins.append([start, end])

# Generate histogram data
data = []
for i, (start, end) in enumerate(bins):
    data.extend([np.random.uniform(start, end) for _ in range(frequencies[i])])

# Plot histogram
plt.hist(data, bins=len(bins), range=(bins[0][0], bins[-1][1]), edgecolor='black', color = 'red')

# Add value annotations to bars
for i, freq in enumerate(frequencies):
    plt.text((bins[i][0] + bins[i][1]) / 2, freq, str(freq), ha='center', va='bottom')

# Set labels and title
plt.xlabel('Temperature')
plt.ylabel('Frequency')
plt.title('Histogram For Weather Index')

# Show plot
plt.show()
