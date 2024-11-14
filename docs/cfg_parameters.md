# Configuration Parameters

This document list all possible input parameters for configuration file `.cfg`. List was created based on the notes in the code of DESTINY. In case of any ambiguity see: InputParameter.cpp, InputParameter.h. Each parameter is described in the following format:

- **Parameter Name**: Description of what the parameter represents, Unit: unit
    Parameter Name: possible_value1; possible_value2; possible_value3; else: default_value

Note: not all the parameters are necessary!

## Necessary parameters 

- **DesignTarget**: Memory design target of the simulation: cache, RAM or CAM
    DesignTarget: cache, RAM, else: CAM

- **OptimizationTarget**: Optimization Target accrodring which DESTINY is choosing best solution (Full means a whole exploration space is saved to csv). 
    OptimizationTarget: Area, ReadLatency, WriteLatency, ReadDynamicEnergy, WriteDynamicEnergy, ReadEDP, WriteEDP, LeakagePower, Full
    For now in the naming convention of the result.csv each optimisation target coresponds to: 
    0 - ReadLatency
	1 - WriteLatency
	2 - ReadDynamicEnergy
    3 - WriteDynamicEnergy
    4 - ReadEDP
    5 - WriteEDP
    6 - LeakagePower
    7 - Area
    8 - Full

- **SecondOptimizationTarget**: optional: same as above in the optimizatiion target expect "Full". In case of the close similarity of two designs according to first optimization target, best is chosen according to defined 2nd optimization target.   

- **OutputFilePrefix**: optional: name of the `result.csv` file

- **ProcessNode**: till 22 nm
    ProcessNode: int 

- **Capacity**: Capacity of designed memory available in units: B, KB, MB (-Capacity (B): int ; -Capacity (KB): int , -Capacity (MB): int)
Capacity: -Capacity (B): int ; -Capacity (KB): int , -Capacity (MB): int

- **MemCellLevel**: Single Level Cell, MultiMLC / TLC continue if SLC (Single Level Cell)
    MemCellLevel: MLC / TLC else: SLC (Single Level Cell)

- **WordWidth (bit)**: size of the word (wordwidth), usally as power of 2 
    WordWidth (bit): int

- **Associativity (for cache only)**: For cache only, see the cache design. The associativity value has to be a power of 2 in this version
    Associativity (for cache only): int

- **Temperature (K)**: Temperature, ranges described in tech
    Temperature (K): int 

- **MaxDriverCurrent (uA)**: The maximum driving current that the wordline/bitline driver can provide
    MaxDriverCurrent (uA): int

- **DeviceRoadmap**: HP / LSTP else LOP - lookup tables in tech:  HP - High performance, LSTP - Low standby power, LOP - Low operating power  (till 22 nm)
    DeviceRoadmap: HP / LSTP else LOP 

- **WriteScheme**: The write schemes
    WriteScheme:    SetBeforeReset 
			        ResetBeforeSet
				    EraseBeforeSet
				    EraseBeforeReset
	                WriteAndVerify
                    else: normal_write
		
- **ReadScheme** :  Read scheme
    ReadScheme:  ReadAndCompare else: normal_read

- **CacheAccessMode**: Access mode to cache
    CacheAccessMode: Sequential, Fast else: normal_access_mode

## Wiring

- **LocalWireType**: Local wire tap acces type
    LocalWireType:  LocalAggressive, 
                    LocalConservative, 
                    SemiAggressive, 
                    SemiConservative, 
                    GlobalAggressive          
                    GlobalConservative

- **LocalWireRepeaterType**: Local wire repeater penalty type
    LocalWireRepeaterType:  RepeatedOpt, 
                            Repeated5%Penalty, 
                            Repeated10%Penalty, 
                            Repeated20%Penalty, 
                            Repeated30%Penalty, 
                            Repeated40%Penalty, 
                            Repeated50%Penalty, 
                            else: RepeatedNone

- **LocalWireUseLowSwing**: If local wire should use low swing
    LocalWireUseLowSwing: Yes else: no 

- **GlobalWireType**: Global wiring type
    GlobalWireType: LocalAggressive, 
                    LocalConservative, 
                    SemiAggressive, 
                    SemiConservative, 
                    GlobalAggressive, 
                    GlobalConservative

- **GlobalWireRepeaterType**: Global wiring reapeter penalty type
    GlobalWireRepeaterType: RepeatedOpt, 
                            Repeated5%Penalty, 
                            Repeated10%Penalty, 
                            Repeated20%Penalty, 
                            Repeated30%Penalty, 
                            Repeated40%Penalty, 
                            Repeated50%Penalty, 
                            else: RepeatedNone

- **GlobalWireUseLowSwing**: If global wire should use low swing
    GlobalWireUseLowSwing: Yes else: no 

## Routing and sensing

- **Routing**: Parameter describing routing to memory - (H-tree descirbed in CACTI documentation) or normal bus access
    Routing: H-tree (else : non-H-tree (bus))

- **InternalSensing**: to be described
    InternalSensing: true (else : false)

## Technology file (.cell)

- **MemoryCellInputFile**: path to `.cell` file - can be provided multiple times for technologies comaprison. DESTINY is able to compare several technologies during the runtime and it chooses the best one (unfortunetly without the scoring, it outputs just the best option)
    MemoryCellInputFile: String (file path) file path to .cell - to provide multiple cell files copy -MemoryCellInputFile: String several times. Destiny will notify you what kind of .cell files it reads. 

- **MaxNmosSize (F)**: Max Nmos size 
    MaxNmosSize (F): int 

## Force Bank, Mat organisation and sense amplifiers 

- **ForceBank3DA (Total AxBxC)**: force 3D design of a single bank (mats), see README 
    ForceBank3DA (Total AxBxC): %dx%dx%d (int x int x int)

- **ForceBank3D (Total AxBxC, Active DxE)**: force a single bank 3D with active mats (from NvSim), see README 
    ForceBank3D (Total AxBxC, Active DxE): %dx%dx%d, %dx%d // int x int x int and int x int

- **ForceBankA (Total AxB)**: force a single bank design (mats), see README
    ForceBankA (Total AxB): %dx%d

- **ForceBank (Total AxB, Active CxD)**: Force a single bank design with active mats (from NvSim), see README
    ForceBank (Total AxB, Active CxD): %dx%d, %dx%d

- **ForceMatA (Total AxB)**: Force organisation of a mat (nxn number of subaarays), see README
    ForceMatA (Total AxB): %dx%d

- **ForceMat (Total AxB, Active CxD)**: Force organisation of a mat (nxn number of subarrays) with active subarrays, see README
    ForceMat (Total AxB, Active CxD): %dx%d, %dx%d

- **ForceMuxSenseAmp**: Force Mux Sense Amplifiers 
    ForceMuxSenseAmp: %d

- **ForceMuxOutputLev1**: Force Number of Mux output level 1 amplifiers
    ForceMuxOutputLev1: %d

- **ForceMuxOutputLev2**: Force Number of Mux output level 2 amplifiers
    ForceMuxOutputLev2: %d 

## Alogrithm options

- **UseCactiAssumption**: Imposing constrains from CACTI on Mat and Subarrays organizations from CACTI publication
    UseCactiAssumption: Yes (else: false) 

- **EnablePruning**: Allow for early pruning during search of solutions
    EnablePruning: Yes 

- **BufferDesignOptimization**: latency, area (else: 1, 1) - to check
    BufferDesignOptimization: latency, area (else: 1, 1)

## Required for Flash: 

- **FlashPageSize (Byte)**: Only for flash - size of the page size
    FlashPageSize (Byte): int 

- **FlashBlockSize (KB)**: Only for flash - size of the block size
    FlashBlockSize (KB): int

## Allowed variation to the best read/write/leakage/area/EDP constrains

- **ApplyReadLatencyConstraint**: floating point 

- **ApplyWriteLatencyConstraint**: floating point 

- **ApplyReadDynamicEnergyConstraint**: floating point 

- **ApplyWriteDynamicEnergyConstraint**: floating point 

- **ApplyLeakageConstraint**: floating point 

- **ApplyAreaConstraint**: floating point 

- **ApplyReadEdpConstraint** : floating point 

- **ApplyWriteEdpConstraint**: floating point 

## TSV

- **PartitionGranularity**: Coarse granularity of Fine granurality, see README
    PartitionGranularity: 0 or 1

- **LocalTSVProjection**: describe (CACTI3DD) 
    LocalTSVProjection: int 

- **GlobalTSVProjection**: describe (CACTI3DD)
    GlobalTSVProjection: int 

- **TSVRedundancy**: describe (CACTI3DD)
    TSVRedundancy: floating point

## 3D design

- **StackedDieCount**: int (3D architecutre see README)

- **MonolithicStackCount**: int (3D architecutre see README)

## Prints

- **PrintAllOptimals**: It is possible to print all optimal designs for particular run
    PrintAllOptimals: true else false

- **AllowDifferentTagTech**: Only for cache: It is possible to allow different tag and cache technology. 
    AllowDifferentTagTech: true else false