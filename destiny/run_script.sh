#!/bin/bash

# Set the base directory to the parent of the 'test' folder
BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# Path to the executable
DESTINY_EXECUTABLE="$BASE_DIR/destiny"

# Base configuration file
BASE_CONFIG="$BASE_DIR/run_11_9/bank_PCRAM_28.cfg"

# Parameters to vary
OPTIMIZATION_TARGETS=("Area" "ReadDynamicEnergy" "ReadEDP" "ReadLatency" "LeakagePower")
STACKED_DIE_COUNTS=(1)
#note that is setted as nxn = m MB of capacity!
FORCE_BANK_AS=("1x1") #"2x2" "4x4" "8x8" "16x16"
WORD_WIDTHS=(8 16 32 64 128 256 512 1024 2048)

# Function to modify configuration file
modify_config() {
    local config_file="$1"
    local param="$2"
    local value="$3"
    sed -i '' "s/^-$param:.*/-$param: $value/" "$config_file"
}

# Function to calculate capacity based on Force_Bank_As
calculate_capacity() {
    local force_bank="$1"
    local n1=$(echo $force_bank | cut -d'x' -f1)
    local n2=$(echo $force_bank | cut -d'x' -f2)
    echo $((n1 * n2))
}

# Create a directory for temporary config files
TEMP_CONFIG_DIR="$BASE_DIR/run_11_9"
mkdir -p "$TEMP_CONFIG_DIR"

# Main loop
for opt_target in "${OPTIMIZATION_TARGETS[@]}"; do
    for stacked_die in "${STACKED_DIE_COUNTS[@]}"; do
        for force_bank in "${FORCE_BANK_AS[@]}"; do
            for word_width in "${WORD_WIDTHS[@]}"; do
                # Calculate capacity based on Force_Bank_As
                capacity=$(calculate_capacity "$force_bank")

                # Set output prefix to be the same as Force_Bank_As
                output_prefix="$force_bank"

                # Create a new configuration file
                config_file="$TEMP_CONFIG_DIR/bank_PCRAM_28.cfg"
                cp "$BASE_CONFIG" "$config_file"

                # Modify the configuration file
                modify_config "$config_file" "OptimizationTarget" "$opt_target"
                modify_config "$config_file" "OutputFilePrefix" "$output_prefix"
                modify_config "$config_file" "StackedDieCount" "$stacked_die"
                modify_config "$config_file" "ForceBankA (Total AxB)" "$force_bank"
                modify_config "$config_file" "WordWidth (bit)" "$word_width"
                modify_config "$config_file" "Capacity (MB)" "$capacity"

                # Run the program with the modified configuration
                echo "Running with: $opt_target, $output_prefix, $stacked_die, $force_bank, $word_width, $capacity MB"
                "$DESTINY_EXECUTABLE" "$config_file"

                # Uncomment the following line if you want to remove the temporary configuration file after each run
                #rm "$config_file"
            done
        done
    done
done