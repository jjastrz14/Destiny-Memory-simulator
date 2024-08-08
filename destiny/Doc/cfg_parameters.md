-DesignTarget : cache, RAM, else: CAM

-OptimizationTarget: ReadLatency, WriteLatency, ReadDynamicEnergy, WriteDynamicEnergy, ReadEDP, WriteEDP, LeakagePower, Area, Full (a whole exploration space generated to csv)

-SecondOptimizationTarget: same as above expect "Full"

-OutputFilePrefix: we don't know

-ProcessNode: int (till 22 nm)

-Capacity: Units: B, KB, MB (-Capacity (B): int ; -Capacity (KB): int , -Capacity (MB): int)

-MemCellLevel: MLC / TLC continue if SLC (Single Level Cell)

-WordWidth (bit): int

-Associativity (for cache only): int ([ERROR] The associativity value has to be a power of 2 in this version)

-Temperature (K): int 

-MaxDriverCurrent (uA): int /* The maximum driving current that the wordline/bitline driver can provide */

-DeviceRoadmap: HP / LSTP else LOP (lookup tables) 	(till 22 nm)
    HP,		/* High performance */
	LSTP,	/* Low standby power */
	LOP,	/* Low operating power */
    EDRAM   /* Embedded DRAM */


-WriteScheme:   SetBeforeReset   /* The write schemes */
			    ResetBeforeSet
				EraseBeforeSet
				EraseBeforeReset
	            WriteAndVerify
                else: normal_write
		
-ReadScheme:  ReadAndCompare else: normal_read

-CacheAccessMode: Sequential, Fast else: normal_access_mode

-LocalWireType: LocalAggressive, 
                LocalConservative, 
                SemiAggressive, 
                SemiConservative, 
                GlobalAggressive          
                GlobalConservative

-LocalWireRepeaterType: RepeatedOpt, 
                        Repeated5%Penalty, 
                        Repeated10%Penalty, 
                        Repeated20%Penalty, 
                        Repeated30%Penalty, 
                        Repeated40%Penalty, 
                        Repeated50%Penalty, 
                        else: RepeatedNone

-LocalWireUseLowSwing: Yes else: no 


-GlobalWireType: LocalAggressive, 
                 LocalConservative, 
                 SemiAggressive, 
                 SemiConservative, 
                 GlobalAggressive, 
                 GlobalConservative

-GlobalWireRepeaterType: RepeatedOpt, 
                        Repeated5%Penalty, 
                        Repeated10%Penalty, 
                        Repeated20%Penalty, 
                        Repeated30%Penalty, 
                        Repeated40%Penalty, 
                        Repeated50%Penalty, 
                        else: RepeatedNone

-GlobalWireUseLowSwing: Yes else: no 

-Routing: H-tree (else : non-H-tree (bus))

-InternalSensing: true (else : false)

-MemoryCellInputFile: String (file path) path to .cell file - can be provided multiple times for technologies comaprison 

-MaxNmosSize (F): int 

-ForceBank3DA (Total AxBxC): %dx%dx%d // int x int x int to force a bank 3D see README 

-ForceBank3D (Total AxBxC, Active DxE): %dx%dx%d, %dx%d // int x int x int and int x int to force a bank 3D with active banks (from NvSim) see README 

-ForceBankA (Total AxB): %dx%d  see README

-ForceBank (Total AxB, Active CxD): %dx%d, %dx%d see README

-ForceMatA (Total AxB): %dx%d see README

-ForceMat (Total AxB, Active CxD): %dx%d, %dx%d see README

-ForceMuxSenseAmp: %d see README

-ForceMuxOutputLev1: %d see README

-ForceMuxOutputLev2: %d see README 

-UseCactiAssumption: Yes (else: false) (imposing constrains from CACTI on Mat and Subarrays organizations) 
/* Use the CACTI assumptions on the array organization */

-EnablePruning: Yes 

-BufferDesignOptimization: latency, area (else: 1, 1)

-FlashPageSize (Byte): int // only for NAND

-FlashBlockSize (KB): int // only for NAND

below The allowed variation to the best read/write/leakage/area/EDP constrains

-ApplyReadLatencyConstraint: floating point 

-ApplyWriteLatencyConstraint: floating point 

-ApplyReadDynamicEnergyConstraint: floating point 

-ApplyWriteDynamicEnergyConstraint: floating point 

-ApplyLeakageConstraint: floating point 

-ApplyAreaConstraint: floating point 

-ApplyReadEdpConstraint: floating point 

-ApplyWriteEdpConstraint: floating point 

-PartitionGranularity: 0 or 1 see README (Coarse granularity of Fine granurality)

-LocalTSVProjection: int 

-GlobalTSVProjection: int 

-TSVRedundancy: floating point

-StackedDieCount: int (3D architecutre see README)

-MonolithicStackCount: int (3D architecutre see README)

-PrintAllOptimals: string (true else false)

-AllowDifferentTagTech: string (true else false)