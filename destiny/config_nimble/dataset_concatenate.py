import pandas as pd
import glob
import re

# Get all CSV files matching the pattern
file_pattern = 'bank_PCRAM_28_*x*_*MB_*_word.csv'
file_list = glob.glob(file_pattern)

# Initialize an empty list to store dataframes
dfs = []

# Regular expression pattern to extract n, r, and k values
pattern = r'bank_PCRAM_28_(\d+)x\1_(\d+)MB_(\d+)_word\.csv'

# Process each file
for file in file_list:
    # Extract n, r, and k values from the filename
    match = re.search(pattern, file)
    if match:
        n, r, k = map(int, match.groups())
        
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

# Save the result to a new CSV file
result.to_csv('summary_PCRAM.csv', index=False, na_rep='N\A')