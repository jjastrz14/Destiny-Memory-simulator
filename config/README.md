# Configuration Files

This directory contains example configuration files for different memory technologies attached to orginal DESTINY repository (see README/NOTICE):

## Directory Structure
- 2D/ - Traditional 2D memory configurations
  - RRAM/
  - eDRAM/
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
- SOTRAM/ - Spin-Orbit Torque RAM configurations
- STTRAM/ - Spin-Transfer Torque RAM configurations
- others: DWM - Domain Wall Memory and others configurations

## File Types
- .cell files: Memory cell parameters
- .cfg files: Memory array configuration parameters

## Usage
see README, remember to define the path to .cell file used in the .cfg file!