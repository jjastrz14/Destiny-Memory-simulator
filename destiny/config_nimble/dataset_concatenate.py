import pandas as pd
import glob
import re

# Get all CSV files matching the pattern
file_pattern = 'bank_PCRAM_28_*x*_*MB_*_word_*.csv'
file_list = glob.glob(file_pattern)

# Initialize an empty list to store dataframes
dfs = []

# Regular expression pattern to extract n, r, and k values
pattern = r'bank_PCRAM_28_(\d+)x(\d+)x\1_(\d+)MB_(\d+)_word_(\d+)\.csv'

# Process each file
for file in file_list:
    # Extract n, r, and k values from the filename
    match = re.search(pattern, file)
    if match:
        n, _, r, k, _ = map(int, match.groups()) 
        
        # Read the CSV file, properly handling 'N\A' values and ensuring all columns are read
        df = pd.read_csv(file, na_values=['N\A'], keep_default_na=False, header=0, index_col=False)
        
        # Replace 'N\A' with pandas NA
        df = df.replace('N\A', pd.NA)
        
        # Add columns for n, r, k, nxn, size, and wordwidth at the beginning
        df.insert(0, 'wordwidth', k)
        df.insert(0, 'nxn', n)
        df.insert(0, 'size MB', r)
    
        # Append to the list of dataframes
        dfs.append(df)

# Concatenate all dataframes
result = pd.concat(dfs, ignore_index=True)

file_extended_path = 'summary_PCRAM_extended.csv'

# Save the result to a new CSV file
result.to_csv(file_extended_path, index=False, na_rep='N\A')


#generation of short summary
'''
keep the columns: 
    Bank Organization
    Optimization Target
    Total Area 
    Mat Area
    Read Latency
    TSV Read Latency
    Mat Read Latency
    Write Total Latency
    Read Bandwidth
    Read Bandwidth per mat
    Write Bandwidth
    Read Dynamic Energy
    TSV Read Dynamic Energy
    Mat Read Dynamic Energy
    Write Dynamic Energy
    Leakage Power
    TSV Leakage Power
    Mat Leakage Power per mat
    Read Dynamic Power
'''

# Generate short summary
short_summary_columns = [
    'Bank Organization', 'size MB', 'wordwidth',
    'Optimization Target', 'Total Area', 'Mat Area',
    'Read Latency', 'TSV Read Latency', 'Mat Read Latency',
    'Write Total Latency', 'Read Bandwidth', 'Read Bandwidth per mat',
    'Write Bandwidth', 'Read Dynamic Energy', 'TSV Read Dynamic Energy',
    'Mat Read Dynamic Energy', 'Write Dynamic Energy', 'Leakage Power',
    'TSV Leakage Power', 'Mat Leakage Power per mat', 'Read Dynamic Power'
]

# Create short summary dataframe
short_summary = result[short_summary_columns]

file_short_path = 'summary_PCRAM_short.csv'

# Save the short summary to a new CSV file
short_summary.to_csv(file_short_path, index=False, na_rep='N\A')

print(f"Full summary saved as {file_extended_path}")
print(f"Short summary saved as {file_short_path}")