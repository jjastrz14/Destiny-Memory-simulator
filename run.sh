#!/bin/bash

# Create main structure
mkdir -p config/{2D,3D}/{RRAM,eDRAM,SRAM}
mkdir -p config/NAND/{MLC,SLC}
mkdir -p config/PCRAM/{base,MLC,NIMBLE/{bank_28,bank_90}}
mkdir -p config/SOTRAM/MLC
mkdir -p config/STTRAM/MLC

# Move files to their new locations
# 2D memories
mv config/sample_2D_RRAM.cfg config/2D/RRAM/RRAM.cfg
mv config/sample_RRAM.cell config/2D/RRAM/RRAM.cell
mv config/sample_RRAM_MLC.cell config/2D/RRAM/RRAM_MLC.cell
mv config/sample_RRAM_MLC.cfg config/2D/RRAM/RRAM_MLC.cfg

mv config/sample_2D_eDRAM.cell config/2D/eDRAM/eDRAM.cell
mv config/sample_2D_eDRAM.cfg config/2D/eDRAM/eDRAM.cfg

mv config/sample_SRAM.cell config/2D/SRAM/SRAM.cell

# 3D memories
mv config/sample_3D_RRAM.cfg config/3D/RRAM/RRAM.cfg
mv config/sample_3D_RRAM_docum.cfg config/3D/RRAM/RRAM_docum.cfg

mv config/sample_3D_eDRAM.cell config/3D/eDRAM/eDRAM.cell
mv config/sample_3D_eDRAM.cfg config/3D/eDRAM/eDRAM.cfg

mv config/sample_SRAM_2layer.cfg config/3D/SRAM/
mv config/sample_SRAM_4layer.cfg config/3D/SRAM/

# NAND
mv config/sample_MLCNAND*.* config/NAND/MLC/
mv config/sample_SLCNAND*.* config/NAND/SLC/

# PCRAM
mv config/sample_PCRAM.cell config/PCRAM/base/
mv config/sample_PCRAM.cfg config/PCRAM/base/
mv config/sample_PCRAM_MLC_*.cell config/PCRAM/MLC/
mv config/sample_PCRAM_NIMBLE_bank_28.* config/PCRAM/NIMBLE/bank_28/
mv config/sample_PCRAM_NIMBLE_bank_90.* config/PCRAM/NIMBLE/bank_90/

# SOTRAM
mv config/sample_SOTRAM.cell config/SOTRAM/
mv config/sample_SOTRAM.cfg config/SOTRAM/
mv config/sample_SOTRAM_MLC.cell config/SOTRAM/MLC/

# STTRAM
mv config/sample_STTRAM.cell config/STTRAM/
mv config/sample_STTRAM.cfg config/STTRAM/
mv config/sample_STTRAM_MLC.cell config/STTRAM/MLC/

# Create README.md
cat > config/README.md << 'EOL'
# Configuration Files

This directory contains configuration files for different memory technologies:

## Directory Structure
- 2D/ - Traditional 2D memory configurations
  - RRAM/
  - eDRAM/
  - SRAM/
- 3D/ - 3D stacked memory configurations
  - RRAM/
  - eDRAM/
  - SRAM/
- NAND/ - NAND Flash configurations
  - MLC/ - Multi-Level Cell
  - SLC/ - Single-Level Cell
- PCRAM/ - Phase Change Memory configurations
  - base/ - Basic configurations
  - MLC/ - Multi-Level Cell
  - NIMBLE/ - NIMBLE specific configurations
- SOTRAM/ - Spin-Orbit Torque RAM configurations
- STTRAM/ - Spin-Transfer Torque RAM configurations

## File Types
- .cell files: Memory cell parameters
- .cfg files: Memory array configuration parameters

## Usage
[Add usage instructions here]
EOL

# Remove empty directories
find config -type d -empty -delete