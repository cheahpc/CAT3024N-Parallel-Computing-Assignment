import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import argparse

# Set up argument parser
parser = argparse.ArgumentParser(description='Generate a histogram from a CSV file.')
parser.add_argument('csv_file', type=str, help='The path to the CSV file containing histogram data.')


# Parse arguments
args = parser.parse_args()

# Load histogram data from file
data = pd.read_csv(args.csv_file, header=None)

# Check if histogram data is empty
if data.empty:
    print(f'No data in {args.csv_file}')
    exit()

# Generate histogram data, no header
data = data.to_numpy()
bins = data[:, :2]
frequencies = data[:, 2].astype(int)

data = []
for i, (start, end) in enumerate(bins):
    data.extend([np.random.uniform(start, end) for _ in range(frequencies[i])])

# Plot histogram
plt.hist(data, bins=len(bins), range=(bins[0][0], bins[-1][1]), edgecolor='black', color='orange')

# Add value annotations to bars
for i, freq in enumerate(frequencies):
    plt.text((bins[i][0] + bins[i][1]) / 2, freq, str(freq), ha='center', va='bottom')

# Take file name as title prefix
tittle = args.csv_file.split('/')[-1].split('.')[0]
tittle = tittle.replace('_', ' ')

# Set labels and title
plt.xlabel('Temperature')
plt.ylabel('Frequency')
plt.title(tittle + ' Weather Index')

# Set title for the window
plt.gcf().canvas.set_window_title(tittle)

# Save plot as image
plt.savefig(f'{tittle}.png', dpi=600)

# Show plot
plt.show()
