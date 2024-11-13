import numpy as np
from itertools import product


def generate_solutions(size_in_mb):
    solutions = []
    # Define the search space
    capacity_divisors = [i for i in range(1, size_in_mb+1) if size_in_mb % i == 0] #capacity in MB or it's divisors
    word_width_options = [2**i for i in range(3, 11)]  # 8 to 1024 wordwidts
    #mux_options_amp = np.arange(1, 257) # only integer values starting from 1 to 256
    #mux_options2 = np.arange(1, 257) # only integer values starting from 1 to 256
    #mux_options3 = np.arange(1, 257) # only integer values starting from 1 to 256
    mux_options_amp = [1,2,4,8,64,128,256] 
    mux_options2 = [1,2,4,8,64,128,256]  
    mux_options3 = [1,2,4,8,64,128,256] 
    # Generate square mat sizes that divide 144 evenly
    mat_sizes = [i for i in range(1, 13) if 144 % (i * i) == 0] #12x12 mats or its equal subcombinations, can be change to any number!
    subbaray_set = [1,2,4,6,8,10,12]
    
    # Generate all combinations
    for (capacity_in_mb, word_width, MuxSenseAmp, MuxOutputLev1, MuxOutputLev2, mat_size, num_subarray, ) in product(
        capacity_divisors, word_width_options, mux_options_amp, mux_options2, mux_options3, mat_sizes, subbaray_set):
        
        num_row_mat = mat_size
        num_col_mat = mat_size
       
        # Fixed values
        associativity = 1 #RAM so it's 1
        stackdiecount = 1 #3D stacking
        num_row_per_set = 1 #RAM so it's 1
        #num_subarray = 1
        
        # Calculations
        capacity = capacity_in_mb * 2**20 * 8 #convert capacity to bits
        num_way_per_row = int(associativity/num_row_per_set)
        num_mats = num_row_mat * num_col_mat
        num_col_mat_active = num_col_mat
        num_row_mat_active = num_row_mat
        N_active_subarrays = num_subarray
        
        Mux_levels = MuxSenseAmp * MuxOutputLev1 * MuxOutputLev2
        num_address_bits = int(np.log2(capacity/word_width/associativity/stackdiecount))
        num_address_bits_mat = num_address_bits - int(np.log2(num_mats/(num_col_mat_active*num_row_mat_active)))
        num_address_bits_gating = int(np.log2(num_subarray/N_active_subarrays))
        num_address_bits_subarray = num_address_bits_mat - num_address_bits_gating
        h = int(np.log2(num_col_mat_active))
        v = int(np.log2(num_row_mat_active))
        row = ((2**(num_address_bits_subarray) * associativity) / Mux_levels * num_way_per_row)
        col = ((word_width / 2**(h+v)) / N_active_subarrays) * Mux_levels * num_way_per_row
        size = num_mats * num_subarray * row * col * stackdiecount
        
        
        # Constrain: capacity = size and col and row are positive integers SINGLE BANK PARAMETERS
        if abs(capacity - size) < 1e-6 and row.is_integer() and col.is_integer() and int(row) > 0 and int(col) > 0:  #small threshold for floating-point comparison
            solutions.append({
                'capacity_in_mb': capacity_in_mb,
                'word_width': word_width,
                'MuxSenseAmp': MuxSenseAmp,
                'MuxOutputLev1': MuxOutputLev1,
                'MuxOutputLev2': MuxOutputLev2,
                'num_mats_col': num_col_mat,
                'num_mats_row': num_row_mat,
                'size': size,
                'row': row,
                'col': col,
                'num_address_bits_subarray': num_address_bits_subarray,
                'num_subbarays': N_active_subarrays
            })
    
    return solutions

# Generate and print solutions
solutions = generate_solutions(size_in_mb = 144)

print(f"Total number of solutions: {len(solutions)}")

#example solutions: 
print(solutions[6900])

# Filter solutions to get unique values of capacity_in_mb and num_mats_col
unique_capacity_in_mb = set()
unique_num_mats_col = set()
unique_word_width = set()

for solution in solutions:
    unique_capacity_in_mb.add(solution['capacity_in_mb'])
    unique_num_mats_col.add(solution['num_mats_col'])
    unique_word_width.add(solution['word_width'])


print(f"Possible values of capacity_in_mb: {sorted(unique_capacity_in_mb)}")
print(f"Possible mat organisation n x n: {sorted(unique_num_mats_col)}")
print(f"Possible values of word_width: {sorted(unique_word_width)}")