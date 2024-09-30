//Copyright (c) 2017-2019, IIT Hyderabad. See LICENSE file in the top-level directory
// This file contains code from NVSim, (c) 2012-2013,  Pennsylvania State University 
//and Hewlett-Packard Company. See LICENSE_NVSim file in the top-level directory.
//No part of DESTINY Project, including this file, may be copied,
//modified, propagated, or distributed except according to the terms
//contained in the LICENSE file.

#ifndef TYPEDEF_H_
#define TYPEDEF_H_
#include <assert.h>
#include <string>
enum MemCellType
{
	SRAM,
	DRAM,
	eDRAM,
	MRAM,
	PCRAM,
	memristor,
	FBRAM,
	SLCNAND,
	MLCNAND,
	DWM
};
inline std::string ToStringMemCellType(MemCellType mc)
{
 if(mc==SRAM) return "SRAM";
 if(mc==DRAM) return "DRAM";
 if(mc==eDRAM) return "eDRAM";
 if(mc==MRAM) return "MRAM";
 if(mc==PCRAM) return "PCM";
 if(mc==memristor) return "RRAM";
 if(mc==FBRAM) return "FBRAM";
 if(mc==SLCNAND) return "SLCNAND";
 if(mc==MLCNAND) return "MLCNAND";
 if(mc==DWM) return "DWM";
 assert(0);
}

enum MemCellLevel
{
	SLC,
	MLC,
	TLC
};
enum CellAccessType
{
	CMOS_access,
	BJT_access,
	diode_access,
	none_access
};

enum DeviceRoadmap
{
	HP,		/* High performance */
	LSTP,	/* Low standby power */
	LOP,	/* Low operating power */
    EDRAM   /* Embedded DRAM */
};
inline std::string ToStringRoadMap(DeviceRoadmap rm)
{
  if(rm == HP) return "HP";
  if(rm == LSTP) return "LSTP";
  if(rm == LOP) return "LOP";
  if(rm == EDRAM) return "EDRAM";
  assert(0);
}

enum WireType
{
	local_aggressive = 0,	/* Width = 2.5F */
	local_conservative = 1,
	semi_aggressive = 2,	/* Width = 4F */
	semi_conservative = 3,
	global_aggressive = 4,	/* Width = 8F */
	global_conservative = 5,
	dram_wordline = 6		/* CACTI 6.5 has this one, but we don't plan to support it at this moment */
};

enum WireRepeaterType
{
	repeated_none = 0,		/* No repeater */
	repeated_opt = 1,       /* Add Repeater, optimal delay */
	repeated_5 = 2,         /* Add Repeater, 5% delay overhead */
	repeated_10 = 3,        /* Add Repeater, 10% delay overhead */
	repeated_20 = 4,        /* Add Repeater, 20% delay overhead */
	repeated_30 = 5,        /* Add Repeater, 30% delay overhead */
	repeated_40 = 6,		/* Add Repeater, 40% delay overhead */
	repeated_50 = 7			/* Add Repeater, 50% delay overhead */
};

enum BufferDesignTarget
{
	latency_first = 0,				/* The buffer will be optimized for latency */
	latency_area_trade_off = 1,		/* the buffer will be fixed to 2-stage */
	area_first = 2					/* The buffer will be optimized for area */
};

enum MemoryType
{
	data_type,
	tag,
	CAM
};

enum RoutingMode
{
	h_tree,
	non_h_tree
};

enum WriteScheme
{
	set_before_reset,
	reset_before_set,
	erase_before_set,
	erase_before_reset,
	write_and_verify,
	normal_write
};

enum ReadScheme
{
	read_and_compare,
	normal_read
};

enum DesignTarget
{
	cache,
	RAM_chip,
	CAM_chip
};

enum OptimizationTarget
{
	read_latency_optimized = 0,
	write_latency_optimized = 1,
	read_energy_optimized = 2,
	write_energy_optimized = 3,
	read_edp_optimized = 4,
	write_edp_optimized = 5,
	leakage_optimized = 6,
	area_optimized = 7,
	full_exploration = 8,
	null_value = 9 
};
 inline std::string ToStringOptTarget(OptimizationTarget ot)
{
 if(ot == read_latency_optimized) return "RLat";
 if(ot == write_latency_optimized) return "WLat";
 if(ot == read_energy_optimized) return "REnergy";
 if(ot == write_energy_optimized) return "WEnergy";
 if(ot == read_edp_optimized) return "REDP";
 if(ot == write_edp_optimized) return "WEDP";
 if(ot == leakage_optimized) return "LEAKGE";
 if(ot == area_optimized) return "AREA";
 if(ot == full_exploration) return "FULL";
 if(ot == null_value) return "NULLVALUE";
 assert(0);
}

enum CacheAccessMode
{
	normal_access_mode,		/* data array lookup and tag access happen in parallel
								final data block is broadcasted in data array h-tree
								after getting the signal from the tag array */
	sequential_access_mode,	/* data array is accessed after accessing the tag array */
	fast_access_mode		/* data and tag access happen in parallel */
};

enum TSV_type 
{
    Fine = 0,        // ITRS high density
    Coarse = 1,      // Industry reported in 2010
    NUM_TSV_TYPES = 2
};


#endif /* TYPEDEF_H_ */
