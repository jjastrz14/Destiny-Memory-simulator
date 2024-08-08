-MemCellType: SRAM; DRAM; eDRAM; MRAM; PCRAM; FBRAM; memristor; SLCNAND; MLCNAND; DWM; else: SRAM (Memory cell type like MRAM, PCRAM, etc.)

-MemCellLevel: MLC; TLC; else: SLC (Memory cell storage level, SLC, MLC, TLC...)

-ProcessNode: int (Cell original process technology node, Unit: nm)

-CellArea (F^2): double (Cell area, Unit: F^2)

-CellAspectRatio: double (Cell aspect ratio, H/W, 
                            heightInFeatureSize = sqrt(area * aspectRatio);
			                widthInFeatureSize = sqrt(area / aspectRatio))

-ResistanceOnAtSetVoltage (ohm): double (Low resistance state when set voltage is applied)

-ResistanceOffAtSetVoltage (ohm): double (High resistance state when set voltage is applied)

-ResistanceOnAtResetVoltage (ohm): double (Low resistance state when reset voltage is applied)

-ResistanceOffAtResetVoltage (ohm): double (High resistance state when reset voltage is applied)

-ResistanceOnAtReadVoltage (ohm): double (Low resistance state when read voltage is applied)

-ResistanceOffAtReadVoltage (ohm): double (High resistance state when read voltage is applied)

-ResistanceOnAtHalfReadVoltage (ohm): double (Low resistance state when 1/2 read voltage is applied)

-ResistanceOffAtHalfReadVoltage (ohm): double (High resistance state when 1/2 read voltage is applied)

-ResistanceOnAtHalfResetVoltage (ohm): double (Low resistance state when 1/2 reset voltage is applied)

-ResistanceOn (ohm): double (Turn-on resistance)

-ResistanceOff (ohm): double (Turn-off resistance)

-CapacitanceOn (F): double (Cell capacitance when memristor is on)

-CapacitanceOff (F): double (Cell capacitance when memristor is off)

-GateOxThicknessFactor: double (The oxide thickness of FBRAM could be larger than the traditional SOI MOS)

-SOIDeviceWidth (F): double (The gate width of SOI device as FBRAM)

-ReadMode: voltage; else: current (true = voltage-mode, false = current-mode)

-ReadVoltage (V): double (Read voltage)

-ReadCurrent (uA): double (Read current)

-ReadPower (uW): double (Read power per bitline (uW))

-ReadPulse (ns): double (Read pulse)

-WordlineBoostRatio: double (TO-DO: function not realized: ratio of boost wordline voltage to vdd)

-MinSenseVoltage (mV): double (Minimum sense voltage)

-ResetMode: voltage; else: current (true = voltage-mode, false = current-mode)

-ResetVoltage (V): "vdd" or double : string/double (Reset voltage) 
                    If value is "vdd", it means the reset voltage should be set to a predefined voltage level stored in tech->vdd. This is a special case where the reset voltage is tied to a technology-specific voltage level, likely defined elsewhere in the program. If tmp is not "vdd",floating-point number directly goes into resetVoltage. 


-ResetCurrent (uA): double (Reset current)

-ResetPulse (ns): double (Reset pulse duration (ns))

-ResetEnergy (pJ): double (Reset energy per cell (pJ))

-SoftResetVoltage (V): "vdd" or double (Reset voltage) as in the ResetVolatege

-SoftResetCurrent (uA): double (Reset current)

-SoftResetPulse (ns): double (Reset pulse duration (ns))

-SoftResetEnergy (pJ): double (Reset energy per cell (pJ))

-SetMode: voltage; else: current (true = voltage-mode, false = current-mode)

-SetVoltage (V): "vdd" or double : (Set voltage) "vdd" or double (Reset voltage) as in the ResetVolatege

-SetCurrent (uA): double (Set current)

-SetPulse (ns): double (Set pulse duration (ns))

-SetEnergy (pJ): double (Set energy per cell (pJ))

-SoftSetVoltage (V): "vdd" or double : (Set voltage) "vdd" or double (Reset voltage) as in the ResetVolatege

-SoftSetCurrent (uA): double (Set current)

-SoftSetPulse (ns): double (Set pulse duration (ns))

-SoftSetEnergy (pJ): double (Set energy per cell (pJ))

-AverageIterations: doube (ignored for SLC)

-Interval (ns): double (ignored for SLC)

-Stitching: int (If non-zero, add stitching overhead for every x cells)

-AccessType: CMOS; BJT; diode; else: none (Cell access type: CMOS, BJT, or diode, none means that DESTINY chooses best AccessType)

-AccessCMOSWidth (F): double (The input of CMOS access transistor width is ignored because the cell is not CMOS-accessed.)

-VoltageDropAccessDevice (V): double 

-LeakageCurrentAccessDevice (uA): double 

-DRAMCellCapacitance (F): double (only for DRAM)

-SRAMCellNMOSWidth (F): double (only for SRAM)

-SRAMCellPMOSWidth (F): double (only for SRAM)

-ReadFloating: true (else is false)

-FlashEraseVoltage (V): double (The erase voltage, Unit: V, highest W/E voltage in ITRS sheet, only for flash)

-FlashProgramVoltage (V): double (The program voltage, Unit: V, only for flash)

-FlashPassVoltage (V): double (The voltage applied on the unselected wordline within the same block during programming, Unit: V, only for flash)

-FlashEraseTime (ms): double (The flash erase time, Unit: s, only for flash)

-FlashProgramTime (us): double (The SLC flash program time, Unit: s, only for flash)

-GateCouplingRatio: double (The ratio of control gate to total floating gate capacitance, only for flash)

-RetentionTime (us): double (Cell time to data loss (us), only for eDRAM)

-Temperature (K): double (Temperature for which the cell input values are valid , only for eDRAM)

-TapeLength (bit): double (only for DWM)

-PortDistance (bit): double (only for DWM)

-TapePerGroup: double (only for DWM)

-ShiftCurrent (uA): double (only for DWM)

-ShiftPulse (ns): double (only for DWM)

-ShiftEnergy (pJ): double (only for DWM)