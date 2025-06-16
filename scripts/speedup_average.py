import csv
import argparse

# Set up argument parser
parser = argparse.ArgumentParser(description="Calculate the average Speedup and write to a new CSV file.")
parser.add_argument("input_file", type=str, help="Path to the input CSV file.")
parser.add_argument("output_file", type=str, help="Path to the output CSV file.")
args = parser.parse_args()

# Input and output file paths from command line arguments
input_file = args.input_file
output_file = args.output_file

# Initialize variables to store data
width = None
samples = None
numberGrid = None
speedup_values = []

# Read the input CSV file
with open(input_file, mode="r") as infile:
    reader = csv.DictReader(infile)
    for row in reader:
        if width is None and samples is None:
            # Extract Width and Samples from the first row
            width = row["Width"]
            samples = row["Samples"]
            numberGrid = row["NumberGrid"]
        # Append the Speedup value for averaging
        speedup_values.append(float(row["Speedup"]))

# Calculate the average Speedup
average_speedup = sum(speedup_values) / len(speedup_values)

# Write the output CSV file
with open(output_file, mode="a", newline="") as outfile:
    writer = csv.writer(outfile)
    # Write the header
    writer.writerow(["Width", "Samples", "NumberGrid", "AverageSpeedup"])
    # Write the data row
    writer.writerow([width, samples, numberGrid, average_speedup])

print(f"Average speedup calculated and written to {output_file}.")
