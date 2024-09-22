import glob
import re
import pandas as pd
import numpy as np

def convert_to_pj(value):
    if pd.isna(value):
        return np.nan
    if isinstance(value, str):
        if 'pJ' in value:
            return float(value.replace('pJ', ''))
        elif 'nJ' in value:
            return float(value.replace('nJ', '')) * 1000
        elif 'uJ' in value:
            return float(value.replace('uJ', '')) * 1000000
        elif 'mJ' in value:
            return float(value.replace('mJ', '')) * 1000000000
        elif 'J' in value:
            return float(value.replace('J', '')) * 1000000000000
        else: 
            raise ValueError(f"Unknown energy unit in value: {value}")
    return float(value)

def convert_to_mw(value):
    if pd.isna(value):
        return np.nan
    if isinstance(value, str):
        if 'mW' in value:
            return float(value.replace('mW', ''))
        elif 'nW' in value:
            return float(value.replace('nW', '')) / 1000000
        elif 'uW' in value:
            return float(value.replace('uW', '')) / 1000
        elif 'W' in value:
            return float(value.replace('W', '')) * 1000
        else: 
            raise ValueError(f"Unknown power unit in value: {value}")
    return float(value)

def convert_to_um2(value):
    if pd.isna(value):
        return np.nan
    if isinstance(value, str):
        if 'mm^2' in value:
            return float(value.replace('mm^2', '')) * 1000000
        elif 'um^2' in value:
            return float(value.replace('um^2', ''))
        else: 
            raise ValueError(f"Unknown area unit in value: {value}")
    return float(value)

def convert_to_mbs(value):
    if pd.isna(value):
        return np.nan
    if isinstance(value, str):
        if 'GB/s' in value:
            return float(value.replace('GB/s', '')) * 1000
        elif 'MB/s' in value:
            return float(value.replace('MB/s', ''))
        else: 
            raise ValueError(f"Unknown bandwidth unit in value: {value}")
    return float(value)

# Get all CSV files matching the pattern
file_pattern = 'config_DATE/PCRAM_28_*x*_*MB_*_word_*_routing_*.csv'
file_list = glob.glob(file_pattern)

# Initialize an empty list to store dataframes
dfs = []

# Updated regular expression pattern to extract n1, n2, r, and k values
pattern = r'bank_PCRAM_28_(\d+)x(\d+)_(\d+)MB_(\d+)_word_(\d+)\.csv'

# Process each file
for file in file_list:
    # Extract n1, n2, r, and k values from the filename
    match = re.search(pattern, file)
    if match:
        n1, n2, r, k, _ = map(int, match.groups()) 
        
        # Read the CSV file, properly handling 'N\A' values and ensuring all columns are read
        df = pd.read_csv(file, na_values=['N\A'], keep_default_na=False, header=0, index_col=False)
        
        # Replace 'N\A' with pandas NA
        df = df.replace('N\A', pd.NA)
        
        # Add columns for n1xn2, r, k, size, and wordwidth at the beginning
        df.insert(0, 'wordwidth', k)
        df.insert(0, 'nxn', f'{n1}x{n2}')
        df.insert(0, 'size MB', r)
    
        # Append to the list of dataframes
        dfs.append(df)

# Concatenate all dataframes
result = pd.concat(dfs, ignore_index=True)

file_extended_path = 'summary_PCRAM_non_htree_extended.csv'

# Save the result to a new CSV file
result.to_csv(file_extended_path, index=False, na_rep='N\A')

# Generate short summary
short_summary_columns = [
    'Bank Organization', 'size MB', 'wordwidth', 'nxn',
    'Optimization Target', 'Total Area', 'Mat Area',
    'Read Latency', 'TSV Read Latency', 'Mat Read Latency',
    'Write Total Latency', 'Read Bandwidth', 'Read Bandwidth per mat',
    'Write Bandwidth', 'Read Dynamic Energy', 'TSV Read Dynamic Energy',
    'Mat Read Dynamic Energy', 'Write Dynamic Energy', 'Leakage Power',
    'TSV Leakage Power', 'Mat Leakage Power per mat', 'Read Dynamic Power',
    'Read Dynamic Power per mat','TSV Dynamic Power'
]

# Create short summary dataframe
short_summary = result[short_summary_columns].copy()

file_short_path = 'summary_PCRAM_non_htree_short.csv'

# Save the short summary to a new CSV file
short_summary.to_csv(file_short_path, index=False, na_rep='N\A')

short_shorter_summary_columns = [
    'nxn', 'size MB', 'wordwidth',
    'Optimization Target', 'Total Area',
    'Read Bandwidth per mat', 'Mat Read Dynamic Energy',
    'Read Dynamic Power per mat','Leakage Power'
]

standardized_summary = result[short_shorter_summary_columns].copy()

# Convert units
standardized_summary.loc[:, 'Total Area'] = standardized_summary['Total Area'].apply(convert_to_um2)
standardized_summary.loc[:, 'Read Bandwidth per mat'] = standardized_summary['Read Bandwidth per mat'].apply(convert_to_mbs)
#standardized_summary.loc[:, 'Write Bandwidth'] = standardized_summary['Write Bandwidth'].apply(convert_to_mbs)
standardized_summary.loc[:, 'Mat Read Dynamic Energy'] = standardized_summary['Mat Read Dynamic Energy'].apply(convert_to_pj)
#standardized_summary.loc[:, 'Write Dynamic Energy'] = standardized_summary['Write Dynamic Energy'].apply(convert_to_pj)
standardized_summary.loc[:, 'Leakage Power'] = standardized_summary['Leakage Power'].apply(convert_to_mw)
standardized_summary.loc[:, 'Read Dynamic Power per mat'] = standardized_summary['Read Dynamic Power per mat'].apply(convert_to_mw)

# Add the "mem req" column
standardized_summary['mem req'] = standardized_summary['size MB'] * (144 / standardized_summary['size MB'])

# Scale the rows based on 'size MB' and create new columns for scaled values
columns_to_scale = [
    'Total Area',
    'Leakage Power'
]

for column in columns_to_scale:
    new_column_name = f"{column} x number banks"
    standardized_summary[new_column_name] = standardized_summary[column] * (144 / standardized_summary['size MB'])

# Rename columns to include units
standardized_summary = standardized_summary.rename(columns={
    'Total Area': 'Total Area (um^2)',
    'Total Area x number banks': 'Total Area x number banks (um^2)',
    'Read Bandwidth per mat': 'Read Bandwidth per mat (MB/s)',
    'Mat Read Dynamic Energy': 'Read Dynamic Energy per mat (pJ)',
    'Leakage Power': 'Leakage Power (mW)',
    'Leakage Power x number banks': 'Leakage Power x number banks (mW)',
    'Read Dynamic Power per mat': 'Read Dynamic Power per mat (mW)'
})

# Reorder columns to place 'mem req' after 'size MB' and scaled columns after original ones
columns_order = standardized_summary.columns.tolist()
size_mb_index = columns_order.index('size MB')
columns_order.insert(size_mb_index + 1, columns_order.pop(columns_order.index('mem req')))

# Move scaled columns after their original counterparts
for column in columns_to_scale:
    original_index = columns_order.index(f"{column} (um^2)" if column == 'Total Area' else f"{column} (mW)")
    scaled_column = f"{column} x number banks ({('um^2' if column == 'Total Area' else 'mW')})"
    columns_order.insert(original_index + 1, columns_order.pop(columns_order.index(scaled_column)))

standardized_summary = standardized_summary[columns_order]

file_standardized_path = 'summary_PCRAM_short_non_htree_standardized.csv'

# Save the standardized summary to a new CSV file
standardized_summary.to_csv(file_standardized_path, index=False, na_rep='N\A')

print(f"Full summary saved as {file_extended_path}")
print(f"Short summary saved as {file_short_path}")
print(f"Standardized short summary saved as {file_standardized_path}")