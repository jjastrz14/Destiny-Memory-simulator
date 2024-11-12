#!/bin/bash

# Create directory structure
mkdir -p src/{core,tsv,decoders,peripherals,utils}
mkdir -p include/{core,tsv,decoders,peripherals,utils}
mkdir -p build results examples docs

# Move source files (.cpp)
# Core components
mv MemCell.cpp Bank.cpp BankWithHtree.cpp BankWithoutHtree.cpp Mat.cpp SubArray.cpp src/core/
mv FunctionUnit.cpp src/core/

# Decoders
mv BasicDecoder.cpp PredecodeBlock.cpp RowDecoder.cpp src/decoders/

# Peripherals
mv Precharger.cpp SenseAmp.cpp Comparator.cpp OutputDriver.cpp Mux.cpp src/peripherals/
mv TSV.cpp src/tsv/

# Utils
mv Technology.cpp Wire.cpp Result.cpp InputParameter.cpp formula.cpp src/utils/

# Main file
mv main.cpp src/

# Move header files (.h)
# Core components
mv MemCell.h Bank.h BankWithHtree.h BankWithoutHtree.h Mat.h SubArray.h include/core/
mv FunctionUnit.h include/

# Decoders
mv BasicDecoder.h PredecodeBlock.h RowDecoder.h include/decoders/

# Peripherals
mv Precharger.h SenseAmp.h Comparator.h OutputDriver.h Mux.h TSV.h include/peripherals/

mv TSV.h include/tsv/

# Utils
mv Technology.h Wire.h Result.h InputParameter.h formula.h global.h constant.h typedef.h macros.h include/utils/