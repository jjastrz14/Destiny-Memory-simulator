# Memory Cell Parameters

This document list all possible input parameters for memory cells. List was created based on the notes in the code of DESTINY. In case of any ambiguity see: InputParameter.cpp, InputParameter.h. Each parameter is described in the following format:

- **Parameter Name**: Description of what the parameter represents, Unit: unit
    Parameter Name: possible_value1; possible_value2; possible_value3; else: default_value

## Basic cell configuration 
- **MemCellType**: Memory cell technology type
    MemCellType: SRAM; DRAM; eDRAM; MRAM; PCRAM; FBRAM; memristor; SLCNAND; MLCNAND; DWM; else: SRAM 

- **MemCellLevel**: Memory cell storage level: SLC, MLC, TLC
    MemCellLevel: MLC; TLC; else: SLC

- **ProcessNode**: Cell original process technology node, Unit: nm
    ProcessNode: int 

## Physical Properties
- **CellArea (F^2)**: Cell area, Unit: F^2
    CellArea (F^2): double

- **CellAspectRatio**: Cell aspect ratio, H/W, heightInFeatureSize = sqrt(area * aspectRatio); widthInFeatureSize = sqrt(area / aspectRatio)
    CellAspectRatio: double

- **ResistanceOnAtSetVoltage (ohm)**: Low resistance state when set voltage is applied, unit: ohm
    ResistanceOnAtSetVoltage (ohm): double

- **ResistanceOffAtSetVoltage (ohm)**: High resistance state when set voltage is applied, unit: ohm
    ResistanceOffAtSetVoltage (ohm): double

- **ResistanceOnAtResetVoltage (ohm)**: Low resistance state when reset voltage is applied, unit: ohm
   ResistanceOnAtResetVoltage (ohm): double

- **ResistanceOffAtResetVoltage (ohm)**: High resistance state when reset voltage is applied, unit: ohm
    ResistanceOffAtResetVoltage (ohm): double

- **ResistanceOnAtReadVoltage (ohm)**: Low resistance state when read voltage is applied, unit: ohm
    ResistanceOnAtReadVoltage (ohm): double

- **ResistanceOffAtReadVoltage (ohm)**: High resistance state when read voltage is applied, unit: ohm 
    ResistanceOffAtReadVoltage (ohm): double

- **ResistanceOnAtHalfReadVoltage (ohm)**: Low resistance state when 1/2 read voltage is applied, unit: ohm
    ResistanceOnAtHalfReadVoltage (ohm): double

- **ResistanceOffAtHalfReadVoltage (ohm)**: High resistance state when 1/2 read voltage is applied, unit: ohm
    ResistanceOffAtHalfReadVoltage (ohm): double

- **ResistanceOnAtHalfResetVoltage (ohm)**: Low resistance state when 1/2 reset voltage is applied, unit: ohm
    ResistanceOnAtHalfResetVoltage (ohm): double 

- **ResistanceOn (ohm)**: Turn-on resistance, unit: ohm
    ResistanceOn (ohm): double

- **ResistanceOff (ohm)**: Turn-off resistance, unit: ohm
    ResistanceOff (ohm): double

- **CapacitanceOn (F)**: Cell capacitance when memristor is on, unit: F
    CapacitanceOn (F): double

- **CapacitanceOff (F)**: Cell capacitance when memristor is off, unit: F
    CapacitanceOff (F): double

- **ReadMode** : voltage-mode or current-mode
    ReadMode: voltage; else: current (true = voltage-mode, false = current-mode)

- **ReadVoltage (V)**: Read voltage, unit: V
    ReadVoltage (V): double

- **ReadCurrent (uA)**: Read current, unit: uA
    ReadCurrent (uA): double

- **ReadPower (uW)**: Read power per bitline, unit: uW
    ReadPower (uW): double

- **ReadPulse (ns)**: Read pulse, unit: ns
    ReadPulse (ns): double

- **WordlineBoostRatio**: TO-DO: function not realized: ratio of boost wordline voltage to vdd, ssems not to work?
    WordlineBoostRatio: double

- **MinSenseVoltage (mV)**: Minimum sense voltage, unit: mV
    MinSenseVoltage (mV): double

- **ResetMode**: voltage-mode or current-mode
    ResetMode: voltage; else: current (true = voltage-mode, false = current-mode)

- **ResetVoltage (V)**: Reset voltage, unit: V
    If value is "vdd", it means the reset voltage should be set to a predefined voltage level stored in tech->vdd. This is a special case where the reset voltage is tied to a technology-specific voltage level, likely defined elsewhere in the program. If tmp is not "vdd",floating-point number directly goes into resetVoltage. 
    ResetVoltage (V): "vdd" or double : string/double (Reset voltage) 
                   
- **ResetCurrent (uA)**: Reset current, unit: uA
    ResetCurrent (uA): double

- **ResetPulse (ns)**: Reset pulse duration, unit: ns
    ResetPulse (ns): double

- **ResetEnergy (pJ)**: Reset energy per cell, unit: pJ
    ResetEnergy (pJ): double

- **SoftResetVoltage (V)**: same as in the ResetVolatege, unit: V
    SoftResetVoltage (V): "vdd" or double

- **SoftResetCurrent (uA)**: Reset current, unit: uA
    SoftResetCurrent (uA): double

- **SoftResetPulse (ns)**: Reset pulse duration, unit: ns
    SoftResetPulse (ns): double

- **SoftResetEnergy (pJ)**: Reset energy per cell, unit: pJ
    SoftResetEnergy (pJ): double

- **SetMode**: voltage-mode or current-mode
    SetMode: voltage; else: current (true = voltage-mode, false = current-mode)

- **SetVoltage (V)**: Set voltage; rules same as in the ResetVolatege, unit: V
    SetVoltage (V): "vdd" or double

- **SetCurrent (uA)**: Set current, unit: uA
    SetCurrent (uA): double

- **SetPulse (ns)**: Set pulse duration, unit: ns
    SetPulse (ns): double

- **SetEnergy (pJ)**: Set energy per cell, unit: pJ
    SetEnergy (pJ): double

- **SoftSetVoltage (V)**: SoftSet voltage rules the same as in the ResetVolatege, unit: V
    SoftSetVoltage (V): "vdd" or double

- **SoftSetCurrent (uA)**: Softset current, unit: uA
    SoftSetCurrent (uA): double

- **SoftSetPulse (ns)**: Set pulse duration, unit: ns
    SoftSetPulse (ns): double

- **SoftSetEnergy (pJ)**: Set energy per cell, unit: pJ
    SoftSetEnergy (pJ): double

- **AverageIterations**: ignored for SLC
    AverageIterations: double

- **Interval (ns)**: ignored for SLC, unit: ns
    Interval (ns): double

- **Stitching**: (If non-zero, add stitching overhead for every x cells)
    Stitching: int

- **AccessType**: Cell access type: CMOS, BJT, or diode, none means that DESTINY chooses best AccessType
    AccessType: CMOS; BJT; diode; else: none

- **AccessCMOSWidth (F)**: The input of CMOS access transistor width is ignored because the cell is not CMOS-accessed.
    AccessCMOSWidth (F): double

- **VoltageDropAccessDevice (V)**:  Voltage drop in acess device
    VoltageDropAccessDevice (V): double 

- **LeakageCurrentAccessDevice (uA)**: Leakage current access device
    LeakageCurrentAccessDevice (uA): double 

## FBRAM

- **GateOxThicknessFactor**: FBRAM: The oxide thickness of FBRAM could be larger than the traditional SOI MOS
    GateOxThicknessFactor: double

- **SOIDeLviceWidth (F)**: FBRAM: The gate width of SOI device as FBRAM
    SOIDeLviceWidth (F): double

## DRAM

- **DRAMCellCapacitance (F)**: only for DRAM, unit: F
    DRAMCellCapacitance (F): double 

## eDRAM

- **RetentionTime (us)**: Only for eDRAM, Cell time to data loss, unit: us
    RetentionTime (us): double

- **Temperature (K)**: Only for eDRAM, temperature for which the cell input values are valid, unit: K
    Temperature (K): double 

## SRAM

- **SRAMCellNMOSWidth (F)**: only for SRAM
    SRAMCellNMOSWidth (F): double

- **SRAMCellPMOSWidth (F)**: only for SRAM
    SRAMCellPMOSWidth (F): double

## FLASH

- **ReadFloating**:
    ReadFloating: true (else is false)

- **FlashEraseVoltage (V)**: Only for flash: The erase voltage, Unit: V, highest W/E voltage in ITRS sheet
    FlashEraseVoltage (V): double

- **FlashProgramVoltage (V)**: Only for flash: the program voltage, unit: V
    FlashProgramVoltage (V): double

- **FlashPassVoltage (V)**: Only for flash: the voltage applied on the unselected wordline within the same block during programming, unit: V
    FlashPassVoltage (V): double

- **FlashEraseTime (ms)**: Only for flash: the flash erase time, unit: s
    FlashEraseTime (ms): double

- **FlashProgramTime (us)**: Only for flash, the SLC flash program time, unit: s
    FlashProgramTime (us): double

- **GateCouplingRatio**: Only for flash, The ratio of control gate to total floating gate capacitance
    GateCouplingRatio: double 

## DWM 

- **TapeLength (bit)**: Only for DWM, in bits 
    TapeLength (bit): double

- **PortDistance (bit)**: Only for DWM, in bit
    PortDistance (bit): double

- **TapePerGroup**: Only for DWM
    TapePerGroup: double

- **ShiftCurrent (uA)**: only for DWM, unit: uA
    ShiftCurrent (uA): double

- **ShiftPulse (ns)**: only for DWM, unit: ns
    ShiftPulse (ns): double

- **ShiftEnergy (pJ)**: only for DWM, unit: pJ
    ShiftEnergy (pJ): double