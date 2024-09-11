//Copyright (c) 2017-2019, IIT Hyderabad. See LICENSE file in the top-level directory
//Copyright (c) 2015-2016, UT-Battelle, LLC. See LICENSE file in the top-level directory
// This file contains code from NVSim, (c) 2012-2013,  Pennsylvania State University 
//and Hewlett-Packard Company. See LICENSE_NVSim file in the top-level directory.
//No part of DESTINY Project, including this file, may be copied,
//modified, propagated, or distributed except according to the terms
//contained in the LICENSE file.


#include "Result.h"
#include "global.h"
#include "formula.h"
#include "macros.h"
#include <iostream>
#include <fstream>

#include <sstream>
using namespace std;
	 

Result::Result() {
	
    if (inputParameter->routingMode == h_tree)
		bank = new BankWithHtree();
	else
		bank = new BankWithoutHtree();
	localWire = new Wire();
	globalWire = new Wire();

	/* initialize the worst case */
	bank->readLatency = invalid_value;
	bank->writeLatency = invalid_value;
	bank->readDynamicEnergy = invalid_value;
	bank->writeDynamicEnergy = invalid_value;
	bank->leakage = invalid_value;
	bank->height = invalid_value;
	bank->width = invalid_value;
	bank->area = invalid_value;

	/* No constraints */
	limitReadLatency = invalid_value;
	limitWriteLatency = invalid_value;
	limitReadDynamicEnergy = invalid_value;
	limitWriteDynamicEnergy = invalid_value;
	limitReadEdp = invalid_value;
	limitWriteEdp = invalid_value;
	limitArea = invalid_value;
	limitLeakage = invalid_value;

	/* Default read latency optimization */
	optimizationTarget = read_latency_optimized;

    cellTech = NULL;
}

Result::~Result() {
	// TODO Auto-generated destructor stub
	if (bank)
		delete bank;
	if (Result::localWire)
		delete Result::localWire;
	if (Result::globalWire)
		delete Result::globalWire;
}

void Result::reset() {
	bank->readLatency = invalid_value;
	bank->writeLatency = invalid_value;
	bank->readDynamicEnergy = invalid_value;
	bank->writeDynamicEnergy = invalid_value;
	bank->leakage = invalid_value;
	bank->height = invalid_value;
	bank->width = invalid_value;
	bank->area = invalid_value;
}

/*bool Result::compareAndUpdate(Result &newResult) {
    bool toUpdate = false;

	if (newResult.bank->readLatency <= limitReadLatency && newResult.bank->writeLatency <= limitWriteLatency
			&& newResult.bank->readDynamicEnergy <= limitReadDynamicEnergy && newResult.bank->writeDynamicEnergy <= limitWriteDynamicEnergy
			&& newResult.bank->readLatency * newResult.bank->readDynamicEnergy <= limitReadEdp
			&& newResult.bank->writeLatency * newResult.bank->writeDynamicEnergy <= limitWriteEdp
			&& newResult.bank->area <= limitArea && newResult.bank->leakage <= limitLeakage) {
		switch (optimizationTarget) {
		case read_latency_optimized:
			if 	(newResult.bank->readLatency < bank->readLatency)
				toUpdate = true;
			else if 	( areAlmostEqual(newResult.bank->readLatency,  bank->readLatency))
			{			 
				toUpdate = compareSecondOptTarget(newResult);
			}
			break;
		case write_latency_optimized:
			if 	(newResult.bank->writeLatency < bank->writeLatency)
				toUpdate = true;
			else if	( areAlmostEqual(newResult.bank->writeLatency,  bank->writeLatency))
			{			 
				toUpdate = compareSecondOptTarget(newResult);
			}
			break;
		case read_energy_optimized:
			if 	(newResult.bank->readDynamicEnergy < bank->readDynamicEnergy)
				toUpdate = true;
			break;
		case write_energy_optimized:
			if 	(newResult.bank->writeDynamicEnergy < bank->writeDynamicEnergy)
				toUpdate = true;
			break;
		case read_edp_optimized:
			if 	(newResult.bank->readLatency * newResult.bank->readDynamicEnergy < bank->readLatency * bank->readDynamicEnergy)
				toUpdate = true;
			break;
		case write_edp_optimized:
			if 	(newResult.bank->writeLatency * newResult.bank->writeDynamicEnergy < bank->writeLatency * bank->writeDynamicEnergy)
				toUpdate = true;
			break;
		case area_optimized:
			if 	(newResult.bank->area < bank->area)
				toUpdate = true;
			break;
		case leakage_optimized:
			if 	(newResult.bank->leakage < bank->leakage)
				toUpdate = true;
			break;
		default:	
			// should not happen 
			;
		}
		if (toUpdate) {
			*bank = *(newResult.bank);
			*localWire = *(newResult.localWire);
			*globalWire = *(newResult.globalWire);
		}
	}

    return toUpdate;
}
*/
bool Result::compareAndUpdate(Result &newResult) {
    bool toUpdate = false;

	if (newResult.bank->readLatency <= limitReadLatency && newResult.bank->writeLatency <= limitWriteLatency
			&& newResult.bank->readDynamicEnergy <= limitReadDynamicEnergy && newResult.bank->writeDynamicEnergy <= limitWriteDynamicEnergy
			&& newResult.bank->readLatency * newResult.bank->readDynamicEnergy <= limitReadEdp
			&& newResult.bank->writeLatency * newResult.bank->writeDynamicEnergy <= limitWriteEdp
			&& newResult.bank->area <= limitArea && newResult.bank->leakage <= limitLeakage) {
		switch (optimizationTarget) {
		case read_latency_optimized:
			if 	(newResult.bank->readLatency < bank->readLatency)
				toUpdate = true;
			else if 	( areAlmostEqual(newResult.bank->readLatency,  bank->readLatency))
			{			 
				toUpdate = compareSecondOptTarget(newResult);
			}
			break;
		case write_latency_optimized:
			if 	(newResult.bank->writeLatency < bank->writeLatency)
				toUpdate = true;
			else if	( areAlmostEqual(newResult.bank->writeLatency,  bank->writeLatency))
			{			 
				toUpdate = compareSecondOptTarget(newResult);
			}
			break;
		case read_energy_optimized:
			if 	(newResult.bank->readDynamicEnergy < bank->readDynamicEnergy)
				toUpdate = true;
	                else if	( areAlmostEqual(newResult.bank->readDynamicEnergy,  bank->readDynamicEnergy))
			{			 
				toUpdate = compareSecondOptTarget(newResult);
			}
			break;
		case write_energy_optimized:
			if 	(newResult.bank->writeDynamicEnergy < bank->writeDynamicEnergy)
				toUpdate = true;
			else if	( areAlmostEqual(newResult.bank->writeDynamicEnergy,  bank->writeDynamicEnergy))
			{			 
				toUpdate = compareSecondOptTarget(newResult);
			}
			break;
		case read_edp_optimized:
			if 	(newResult.bank->readLatency * newResult.bank->readDynamicEnergy < bank->readLatency * bank->readDynamicEnergy)
				toUpdate = true;
                        else if	( areAlmostEqual(newResult.bank->readLatency * newResult.bank->readDynamicEnergy,  bank->readLatency * bank->readDynamicEnergy))
			{			 
				toUpdate = compareSecondOptTarget(newResult);
			}
			break;
		case write_edp_optimized:
			if 	(newResult.bank->writeLatency * newResult.bank->writeDynamicEnergy < bank->writeLatency * bank->writeDynamicEnergy)
				toUpdate = true;
                        else if	( areAlmostEqual(newResult.bank->writeLatency * newResult.bank->writeDynamicEnergy,  bank->writeLatency * bank->writeDynamicEnergy))
			{			 
				toUpdate = compareSecondOptTarget(newResult);
			}
			break;
		case area_optimized:
			if 	(newResult.bank->area < bank->area)
				toUpdate = true;
			else if	( areAlmostEqual(newResult.bank->area,  bank->area))
			{			 
				toUpdate = compareSecondOptTarget(newResult);
			}
			break;
		case leakage_optimized:
			if 	(newResult.bank->leakage < bank->leakage)
				toUpdate = true;
			else if	( areAlmostEqual(newResult.bank->leakage,  bank->leakage))
			{			 
				toUpdate = compareSecondOptTarget(newResult);
			}
			break;
		default:	/* Exploration */
			/* should not happen */
			;
		}
		if (toUpdate) {
			*bank = *(newResult.bank);
			*localWire = *(newResult.localWire);
			*globalWire = *(newResult.globalWire);
		}
	}

    return toUpdate;
}

bool Result::compareSecondOptTarget(Result &newResult) {
    bool toUpdate = false;	
		switch (inputParameter->secondOptimizationTarget) {
		case read_latency_optimized:
			if 	(newResult.bank->readLatency < bank->readLatency)
				toUpdate = true;			
			break;
		case write_latency_optimized:
			if 	(newResult.bank->writeLatency < bank->writeLatency)
				toUpdate = true;
			break;
		case read_energy_optimized:
			if 	(newResult.bank->readDynamicEnergy < bank->readDynamicEnergy)
				toUpdate = true;
			break;
		case write_energy_optimized:
			if 	(newResult.bank->writeDynamicEnergy < bank->writeDynamicEnergy)
				toUpdate = true;
			break;
		case read_edp_optimized:
			if 	(newResult.bank->readLatency * newResult.bank->readDynamicEnergy < bank->readLatency * bank->readDynamicEnergy)
				toUpdate = true;
			break;
		case write_edp_optimized:
			if 	(newResult.bank->writeLatency * newResult.bank->writeDynamicEnergy < bank->writeLatency * bank->writeDynamicEnergy)
				toUpdate = true;
			break;
		case area_optimized:
			if 	(newResult.bank->area < bank->area)
				toUpdate = true;
			break;
		case leakage_optimized:
			if 	(newResult.bank->leakage < bank->leakage)
				toUpdate = true;
			break;
	        case null_value:
				toUpdate =false; break;

		default:	/* Exploration */
			toUpdate =false; break;
		} 
    return toUpdate;
}
string Result::printOptimizationTarget() {
    string rv;

    switch (optimizationTarget) {
    case read_latency_optimized:
        rv = "Read Latency";
        break;
    case write_latency_optimized:
        rv = "Write Latency";
        break;
    case read_energy_optimized:
        rv = "Read Energy";
        break;
    case write_energy_optimized:
        rv = "Write Energy";
        break;
    case read_edp_optimized:
        rv = "Read Energy-Delay-Product";
        break;
    case write_edp_optimized:
        rv = "Write Energy-Delay-Product";
        break;
    case area_optimized:
        rv = "Area";
        break;
    case leakage_optimized:
        rv = "Leakage";
        break;
    default:	/* Exploration */
        /* should not happen */
        ;
    }

    return rv;
}

void Result::print(int indent) {
	cout << string(indent, ' ') << endl;
    cout << string(indent, ' ') << "=============" << endl;
    cout << string(indent, ' ') << "   SUMMARY   " << endl;
    cout << string(indent, ' ') << "=============" << endl;
    cout << string(indent, ' ') << "Optimized for: " << printOptimizationTarget() << endl;
    cellTech->PrintCell(indent);
	cout << string(indent, ' ') << endl;
    cout << string(indent, ' ') << "=============" << endl;
    cout << string(indent, ' ') << "CONFIGURATION" << endl;
    cout << string(indent, ' ') << "=============" << endl;
    if (bank->stackedDieCount > 1) {
        cout << string(indent, ' ') << "Bank Organization: " << bank->numRowMat << " x " << bank->numColumnMat << " x " << bank->stackedDieCount << endl;
        cout << string(indent, ' ') << " - Row Activation   : " << bank->numActiveMatPerColumn << " / " << bank->numRowMat << " x 1" << endl;
        cout << string(indent, ' ') << " - Column Activation: " << bank->numActiveMatPerRow << " / " << bank->numColumnMat << " x 1" << endl;
    } else {
        cout << string(indent, ' ') << "Bank Organization: " << bank->numRowMat << " x " << bank->numColumnMat << endl;
        cout << string(indent, ' ') << " - Row Activation   : " << bank->numActiveMatPerColumn << " / " << bank->numRowMat << endl;
        cout << string(indent, ' ') << " - Column Activation: " << bank->numActiveMatPerRow << " / " << bank->numColumnMat << endl;
    }
	cout << string(indent, ' ') << "Mat Organization: " << bank->numRowSubarray << " x " << bank->numColumnSubarray << endl;
	cout << string(indent, ' ') << " - Row Activation   : " << bank->numActiveSubarrayPerColumn << " / " << bank->numRowSubarray << endl;
	cout << string(indent, ' ') << " - Column Activation: " << bank->numActiveSubarrayPerRow << " / " << bank->numColumnSubarray << endl;
	cout << string(indent, ' ') << " - Subarray Size    : " << bank->mat.subarray.numRow << " Rows x " << bank->mat.subarray.numColumn << " Columns" << endl;
	cout << string(indent, ' ') << "Mux Level:" << endl;
	cout << string(indent, ' ') << " - Senseamp Mux      : " << bank->muxSenseAmp << endl;
	cout << string(indent, ' ') << " - Output Level-1 Mux: " << bank->muxOutputLev1 << endl;
	cout << string(indent, ' ') << " - Output Level-2 Mux: " << bank->muxOutputLev2 << endl;
	if (inputParameter->designTarget == cache)
		cout << string(indent, ' ') << " - One set is partitioned into " << bank->numRowPerSet << " rows" << endl;
	cout << string(indent, ' ') << "Local Wire:" << endl;
	cout << string(indent, ' ') << " - Wire Type : ";
	switch (localWire->wireType) {
	case local_aggressive:
		cout << string(indent, ' ') << "Local Aggressive" << endl;
		break;
	case local_conservative:
		cout << string(indent, ' ') << "Local Conservative" << endl;
		break;
	case semi_aggressive:
		cout << string(indent, ' ') << "Semi-Global Aggressive" << endl;
		break;
	case semi_conservative:
		cout << string(indent, ' ') << "Semi-Global Conservative" << endl;
		break;
	case global_aggressive:
		cout << string(indent, ' ') << "Global Aggressive" << endl;
		break;
	case global_conservative:
		cout << string(indent, ' ') << "Global Conservative" << endl;
		break;
	default:
		cout << string(indent, ' ') << "DRAM Wire" << endl;
	}
	cout << string(indent, ' ') << " - Repeater Type: ";
	switch (localWire->wireRepeaterType) {
	case repeated_none:
		cout << string(indent, ' ') << "No Repeaters" << endl;
		break;
	case repeated_opt:
		cout << string(indent, ' ') << "Fully-Optimized Repeaters" << endl;
		break;
	case repeated_5:
		cout << string(indent, ' ') << "Repeaters with 5% Overhead" << endl;
		break;
	case repeated_10:
		cout << string(indent, ' ') << "Repeaters with 10% Overhead" << endl;
		break;
	case repeated_20:
		cout << string(indent, ' ') << "Repeaters with 20% Overhead" << endl;
		break;
	case repeated_30:
		cout << string(indent, ' ') << "Repeaters with 30% Overhead" << endl;
		break;
	case repeated_40:
		cout << string(indent, ' ') << "Repeaters with 40% Overhead" << endl;
		break;
	case repeated_50:
		cout << string(indent, ' ') << "Repeaters with 50% Overhead" << endl;
		break;
	default:
		cout << string(indent, ' ') << "Unknown" << endl;
	}
	cout << string(indent, ' ') << " - Low Swing : ";
	if (localWire->isLowSwing)
		cout << string(indent, ' ') << "Yes" << endl;
	else
		cout << string(indent, ' ') << "No" << endl;
	cout << string(indent, ' ') << "Global Wire:" << endl;
	cout << string(indent, ' ') << " - Wire Type : ";
	switch (globalWire->wireType) {
	case local_aggressive:
		cout << string(indent, ' ') << "Local Aggressive" << endl;
		break;
	case local_conservative:
		cout << string(indent, ' ') << "Local Conservative" << endl;
		break;
	case semi_aggressive:
		cout << string(indent, ' ') << "Semi-Global Aggressive" << endl;
		break;
	case semi_conservative:
		cout << string(indent, ' ') << "Semi-Global Conservative" << endl;
		break;
	case global_aggressive:
		cout << string(indent, ' ') << "Global Aggressive" << endl;
		break;
	case global_conservative:
		cout << string(indent, ' ') << "Global Conservative" << endl;
		break;
	default:
		cout << string(indent, ' ') << "DRAM Wire" << endl;
	}
	cout << string(indent, ' ') << " - Repeater Type: ";
	switch (globalWire->wireRepeaterType) {
	case repeated_none:
		cout << string(indent, ' ') << "No Repeaters" << endl;
		break;
	case repeated_opt:
		cout << string(indent, ' ') << "Fully-Optimized Repeaters" << endl;
		break;
	case repeated_5:
		cout << string(indent, ' ') << "Repeaters with 5% Overhead" << endl;
		break;
	case repeated_10:
		cout << string(indent, ' ') << "Repeaters with 10% Overhead" << endl;
		break;
	case repeated_20:
		cout << string(indent, ' ') << "Repeaters with 20% Overhead" << endl;
		break;
	case repeated_30:
		cout << string(indent, ' ') << "Repeaters with 30% Overhead" << endl;
		break;
	case repeated_40:
		cout << string(indent, ' ') << "Repeaters with 40% Overhead" << endl;
		break;
	case repeated_50:
		cout << string(indent, ' ') << "Repeaters with 50% Overhead" << endl;
		break;
	default:
		cout << string(indent, ' ') << "Unknown" << endl;
	}
	cout << string(indent, ' ') << " - Low Swing : ";
	if (globalWire->isLowSwing)
		cout << string(indent, ' ') << "Yes" << endl;
	else
		cout << string(indent, ' ') << "No" << endl;
	cout << string(indent, ' ') << "Buffer Design Style: ";
	switch (bank->areaOptimizationLevel) {
	case latency_first:
		cout << string(indent, ' ') << "Latency-Optimized" << endl;
		break;
	case area_first:
		cout << string(indent, ' ') << "Area-Optimized" << endl;
		break;
	default:	/* balance */
		cout << string(indent, ' ') << "Balanced" << endl;
	}

	cout << string(indent, ' ') << "=============" << endl;
    cout << string(indent, ' ') << "   RESULT" << endl;
    cout << string(indent, ' ') << "=============" << endl;

	cout << string(indent, ' ') << "Area:" << endl;

	cout << string(indent, ' ') << " - Total Area = " << TO_METER(bank->height) << " x " << TO_METER(bank->width)
			<< " = " << TO_SQM(bank->area) << endl;
	cout << string(indent, ' ') << " |--- Mat Area      = " << TO_METER(bank->mat.height) << " x " << TO_METER(bank->mat.width)
			<< " = " << TO_SQM(bank->mat.area) << "   (" << cell->area * tech->featureSize * tech->featureSize
			* bank->capacity / bank->numRowMat / bank->numColumnMat / bank->mat.area * 100 << "%)" << endl;
	cout << string(indent, ' ') << " |--- Subarray Area = " << TO_METER(bank->mat.subarray.height) << " x "
			<< TO_METER(bank->mat.subarray.width) << " = " << TO_SQM(bank->mat.subarray.area) << "   ("
			<< cell->area * tech->featureSize * tech->featureSize * bank->capacity / bank->numRowMat
			/ bank->numColumnMat / bank->numRowSubarray / bank->numColumnSubarray
			/ bank->mat.subarray.area * 100 << "%)" <<endl;

    if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
        cout << string(indent, ' ') << " |--- TSV Area      = " << TO_SQM(bank->tsvArray.area) << endl;
    } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
        double totalTSVArea = bank->tsvArray.area + bank->mat.tsvArray.area
                              * bank->numColumnMat * bank->numRowMat;
        double areaLogicLayer = bank->mat.areaAllLogicBlocks * bank->numColumnMat
                              * bank->numRowMat;

        cout << string(indent, ' ') << " |--- TSV Area      = " << TO_SQM(totalTSVArea) << endl;
        cout << string(indent, ' ') << " |--- Logic Layer Area = " << TO_SQM(areaLogicLayer) << endl;
    }

	cout << string(indent, ' ') << " - Area Efficiency = " << cell->area * tech->featureSize * tech->featureSize
			* bank->capacity / bank->area * 100 << "%" << endl;

	cout << string(indent, ' ') << "Timing:" << endl;

	cout << string(indent, ' ') << " -  Read Latency = " << TO_SECOND(bank->readLatency) << endl;
    if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
        double totalTSVLatency = (bank->tsvArray.readLatency + bank->tsvArray.writeLatency) * (bank->stackedDieCount-1);
        cout << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(totalTSVLatency) << endl;
    } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
        double totalTSVLatency = bank->tsvArray.readLatency * (bank->stackedDieCount-1)
                               + bank->mat.tsvArray.writeLatency * (bank->stackedDieCount-1);

        cout << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(totalTSVLatency) << endl;
    }
	if (inputParameter->routingMode == h_tree)
		cout << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingReadLatency) << endl;
	else
		cout << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingReadLatency) << endl;
	cout << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.readLatency) << endl;
	cout << string(indent, ' ') << "    |--- Predecoder Latency = " << TO_SECOND(bank->mat.predecoderLatency) << endl;
	cout << string(indent, ' ') << "    |--- Subarray Latency   = " << TO_SECOND(bank->mat.subarray.readLatency) << endl;
	cout << string(indent, ' ') << "       |--- Row Decoder Latency = " << TO_SECOND(bank->mat.subarray.rowDecoder.readLatency) << endl;
	cout << string(indent, ' ') << "       |--- Bitline Latency     = " << TO_SECOND(bank->mat.subarray.bitlineDelay) << ","<<TO_SECOND(bank->mat.subarray.bitlineDelayOn)<<","<< TO_SECOND(bank->mat.subarray.bitlineDelayOff)<< endl;
	if (inputParameter->internalSensing)
		cout << string(indent, ' ') << "       |--- Senseamp Latency    = " << TO_SECOND(bank->mat.subarray.senseAmp.readLatency) << endl;
	cout << string(indent, ' ') << "       |--- Mux Latency         = " << TO_SECOND(bank->mat.subarray.bitlineMux.readLatency
													+ bank->mat.subarray.senseAmpMuxLev1.readLatency
													+ bank->mat.subarray.senseAmpMuxLev2.readLatency) << endl;
	cout << string(indent, ' ') << "       |--- Precharge Latency   = " << TO_SECOND(bank->mat.subarray.precharger.readLatency) << endl;
	cout << string(indent, ' ') << "       |--- Read Pulse   = " << TO_SECOND(cell->readPulse) << endl;

	if (bank->mat.memoryType == tag && bank->mat.internalSenseAmp)
		cout << string(indent, ' ') << "    |--- Comparator Latency  = " << TO_SECOND(bank->mat.comparator.readLatency) << endl;

	if (cell->memCellType == PCRAM || cell->memCellType == FBRAM || cell->memCellType ==MRAM ||
			(cell->memCellType == memristor && (cell->accessType == CMOS_access || cell->accessType == BJT_access || cell->memCellLevel!=SLC))) {
		cout << string(indent, ' ') << " - Write Total Latency = " << TO_SECOND(bank->writeLatency) << endl;
		cout << string(indent, ' ') << " - RESET Latency = " << TO_SECOND(bank->resetLatency) << endl;
		if (cell->memCellType==MRAM && cell->memCellLevel!=SLC){
			cout << string(indent, ' ') << " - RESET Soft Latency = " << TO_SECOND(bank->resetLatencySoft) << endl;
		}
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            cout << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.resetLatency * (bank->stackedDieCount-1)) << endl;
        } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            cout << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.resetLatency * (bank->stackedDieCount-1)) << endl;
        }
		if (inputParameter->routingMode == h_tree)
			cout << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingResetLatency) << endl;
		else
			cout << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingResetLatency) << endl;


		cout << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.resetLatency) << endl;
		cout << string(indent, ' ') << "    |--- Predecoder Latency = " << TO_SECOND(bank->mat.predecoderLatency) << endl;
		cout << string(indent, ' ') << "    |--- Subarray Latency   = " << TO_SECOND(bank->mat.subarray.resetLatency) << endl;
		cout << string(indent, ' ') << "       |--- RESET Pulse Duration = " << TO_SECOND(cell->resetPulse) << endl;
		if (cell->memCellType==MRAM && cell->memCellLevel!=SLC){
			cout << string(indent, ' ') << "       |--- RESET Soft Pulse Duration   = " << TO_SECOND(cell->resetPulseSoft) << endl;
		}
		cout << string(indent, ' ') << "       |--- Row Decoder Latency  = " << TO_SECOND(bank->mat.subarray.rowDecoder.writeLatency) << endl;
		cout << string(indent, ' ') << "       |--- Charge Latency   = " << TO_SECOND(bank->mat.subarray.chargeLatency) << endl;

		cout << string(indent, ' ') << " - SET Latency   = " << TO_SECOND(bank->setLatency) << endl;
		if (cell->memCellType==MRAM && cell->memCellLevel!=SLC){
			cout << string(indent, ' ') << " - SET Soft Latency = " << TO_SECOND(bank->setLatencySoft) << endl;
		}
		if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            cout << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.setLatency * (bank->stackedDieCount-1)) << endl;
        } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            cout << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.setLatency * (bank->stackedDieCount-1)) << endl;
        }
		if (inputParameter->routingMode == h_tree)
			cout << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingSetLatency) << endl;
		else
			cout << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingSetLatency) << endl;
		cout << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.setLatency) << endl;
		cout << string(indent, ' ') << "    |--- Predecoder Latency = " << TO_SECOND(bank->mat.predecoderLatency) << endl;
		cout << string(indent, ' ') << "    |--- Subarray Latency   = " << TO_SECOND(bank->mat.subarray.setLatency) << endl;
		cout << string(indent, ' ') << "       |--- SET Pulse Duration   = " << TO_SECOND(cell->setPulse) << endl;
		if (cell->memCellType==MRAM && cell->memCellLevel!=SLC){
			cout << string(indent, ' ') << "       |--- SET Soft Pulse Duration   = " << TO_SECOND(cell->setPulseSoft) << endl;
		}
		cout << string(indent, ' ') << "       |--- Row Decoder Latency  = " << TO_SECOND(bank->mat.subarray.rowDecoder.writeLatency) << endl;
		cout << string(indent, ' ') << "       |--- Charger Latency      = " << TO_SECOND(bank->mat.subarray.chargeLatency) << endl;


	} else if (cell->memCellType == SLCNAND ||cell->memCellType ==MLCNAND) {
		cout << string(indent, ' ') << " - Erase Latency = " << TO_SECOND(bank->resetLatency) << endl;
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            cout << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.resetLatency * (bank->stackedDieCount-1)) << endl;
        } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            cout << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.resetLatency * (bank->stackedDieCount-1)) << endl;
        }
		if (inputParameter->routingMode == h_tree)
			cout << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingResetLatency) << endl;
		else
			cout << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingResetLatency) << endl;
		cout << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.resetLatency) << endl;
		cout << string(indent, ' ') << " - Programming Latency   = " << TO_SECOND(bank->setLatency) << endl;
		if (inputParameter->routingMode == h_tree)
			cout << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingSetLatency) << endl;
		else
			cout << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingSetLatency) << endl;
		cout << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.setLatency) << endl;
	} else {
		cout << string(indent, ' ') << " - Write Latency = " << TO_SECOND(bank->writeLatency) << endl;
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            cout << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.writeLatency * (bank->stackedDieCount-1)) << endl;
        } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            cout << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.writeLatency * (bank->stackedDieCount-1)) << endl;
        }
		if (inputParameter->routingMode == h_tree)
			cout << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingWriteLatency) << endl;
		else
			cout << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingWriteLatency) << endl;
		cout << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.writeLatency) << endl;
		cout << string(indent, ' ') << "    |--- Predecoder Latency = " << TO_SECOND(bank->mat.predecoderLatency) << endl;
		cout << string(indent, ' ') << "    |--- Subarray Latency   = " << TO_SECOND(bank->mat.subarray.writeLatency) << endl;

		cout << string(indent, ' ') << "       |--- Row Decoder Latency = " << TO_SECOND(bank->mat.subarray.rowDecoder.writeLatency) << endl;
		cout << string(indent, ' ') << "       |--- Charge Latency      = " << TO_SECOND(bank->mat.subarray.chargeLatency) << endl;
	}
    if (cell->memCellType == eDRAM || cell->memCellType == DRAM) {
        cout << string(indent, ' ') << " - Refresh Latency = " << TO_SECOND(bank->refreshLatency) << endl;
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            cout << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.writeLatency * (bank->stackedDieCount-1)) << endl;
        } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            cout << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.writeLatency * (bank->stackedDieCount-1)) << endl;
        }
		if (inputParameter->routingMode == h_tree)
			cout << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingRefreshLatency) << endl;
		else
			cout << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingRefreshLatency) << endl;
		cout << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.refreshLatency) << endl;
		cout << string(indent, ' ') << "    |--- Predecoder Latency = " << TO_SECOND(bank->mat.predecoderLatency) << endl;
		cout << string(indent, ' ') << "    |--- Subarray Latency   = " << TO_SECOND(bank->mat.subarray.refreshLatency) << endl;
    }

    if (cell->memCellType ==DWM) {
        cout << string(indent, ' ') << " - Shift Latency = " << TO_SECOND(bank->shiftLatency) << endl;
    }

	//old version: 
	//double readBandwidth = (double)bank->blockSize /
	//		(bank->mat.subarray.readLatency - bank->mat.subarray.rowDecoder.readLatency
	//		+ bank->mat.subarray.precharger.readLatency) / 8;

	// numReadoudts = numcol/wordwidth_per_subarray_active
	double numReadouts =  (double)bank->mat.subarray.numColumn / ((double)bank->blockSize / 
	(bank->numActiveMatPerColumn * bank->numActiveMatPerRow *  bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)) ;
	cout << string(indent, ' ') << "- OUR number consecutive Readouts per subarray  = " << numReadouts << endl;
	//
	double avg_rowDecoder_latency = (double)bank->mat.subarray.rowDecoder.readLatency/numReadouts;
	cout << string(indent, ' ') << "- OUR avg row decoder Latency   = " << avg_rowDecoder_latency << endl;

	// our correction:
	//Wordwidth / (subarray_latency - rowDecoder_latency + avg_rowDecoder_latency + Precharger_latency) / 8 (B/s)
	//avg version: 

	double readBandwidth = (double)bank->blockSize /
			(bank->mat.subarray.readLatency - bank->mat.subarray.rowDecoder.readLatency
			+ avg_rowDecoder_latency + bank->mat.subarray.precharger.readLatency) / 8;

	cout << string(indent, ' ') << " - Read Bandwidth  = " << TO_BPS(readBandwidth) << endl;

	double readBandwidth_per_mat = (((double)bank->blockSize /(bank->numActiveMatPerColumn * bank->numActiveMatPerRow)) /
			(bank->mat.subarray.readLatency - bank->mat.subarray.rowDecoder.readLatency
			+ avg_rowDecoder_latency + bank->mat.subarray.precharger.readLatency) / 8);

	cout << string(indent, ' ') << " OUR - Read Bandwidth per mat = " << TO_BPS(readBandwidth_per_mat) << endl;
        //cout<<" blocksize "<<bank->blockSize<<" rL "<<bank->mat.subarray.readLatency * 1e6<<" rL1 "<<bank->mat.subarray.rowDecoder.readLatency * 1e6<<" rL2 "<<bank->mat.subarray.precharger.readLatency * 1e6<<"\n";
	double writeBandwidth = (double)bank->blockSize /
			(bank->mat.subarray.writeLatency) / 8;
	cout << string(indent, ' ') << " - Write Bandwidth = " << TO_BPS(writeBandwidth) << endl;

	cout << string(indent, ' ') << "Power:" << endl;

	cout << string(indent, ' ') << " -  Read Dynamic Energy = " << TO_JOULE(bank->readDynamicEnergy) << endl;
	
    if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
        //cerr<<" using bank partitionGranularity as 0\n";
        // address and control bit dynamics + read data output dynamic
        cout << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numReadBits + bank->tsvArray.readDynamicEnergy * bank->tsvArray.numDataBits * (bank->stackedDieCount-1)) << endl;
        //cout<<"SEE bank->tsvArray.writeDynamicEnergy "<<TO_JOULE(bank->tsvArray.writeDynamicEnergy)<<"\n";
	//cout<<"SEE bank->tsvArray.numReadBits "<<bank->tsvArray.numReadBits<<"\n";
	//cout<<"SEE bank->tsvArray.readDynamicEnergy "<<TO_JOULE(bank->tsvArray.readDynamicEnergy)<<"\n";
	//cout<<"SEE bank->tsvArray.numDataBits "<<bank->tsvArray.numDataBits<<"\n";
	
    } 
    else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
        // address bit dynamic + control bit dynamic + read data dynamic
        // TODO: revisit this
        //cerr<<" using bank partitionGranularity as 1\n";
        cout << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->mat.tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * (bank->mat.tsvArray.numAccessBits) + bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->stackedDieCount + bank->tsvArray.readDynamicEnergy * bank->blockSize * (bank->stackedDieCount-1)) << endl;
    }
	if (inputParameter->routingMode == h_tree)
		cout << string(indent, ' ') << " |--- H-Tree Dynamic Energy = " << TO_JOULE(bank->routingReadDynamicEnergy) << endl;
	else
		cout << string(indent, ' ') << " |--- Non-H-Tree Dynamic Energy = " << TO_JOULE(bank->routingReadDynamicEnergy) << endl;
	cout << string(indent, ' ') << " |--- Mat Dynamic Energy    = " << TO_JOULE(bank->mat.readDynamicEnergy) << " per mat" << endl;
	cout << string(indent, ' ') << "    |--- Predecoder Dynamic Energy = " << TO_JOULE(bank->mat.readDynamicEnergy - bank->mat.subarray.readDynamicEnergy
														* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
														<< endl;
	cout << string(indent, ' ') << "    |--- Subarray Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.readDynamicEnergy) << " per active subarray" << endl;
	cout << string(indent, ' ') << "       |--- Row Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.rowDecoder.readDynamicEnergy) << endl;
	cout << string(indent, ' ') << "       |--- Mux Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.bitlineMuxDecoder.readDynamicEnergy
													+ bank->mat.subarray.senseAmpMuxLev1Decoder.readDynamicEnergy
													+ bank->mat.subarray.senseAmpMuxLev2Decoder.readDynamicEnergy) << endl;
	if (cell->memCellType == PCRAM || cell->memCellType == FBRAM || cell->memCellType == MRAM || cell->memCellType == memristor || cell->memCellType ==DWM) {
		cout << string(indent, ' ') << "       |--- Bitline & Cell Read Energy = " << TO_JOULE(bank->mat.subarray.cellReadEnergy) << endl;
	}
	if (inputParameter->internalSensing)
		cout << string(indent, ' ') << "       |--- Senseamp Dynamic Energy    = " << TO_JOULE(bank->mat.subarray.senseAmp.readDynamicEnergy) << endl;
	cout << string(indent, ' ') << "       |--- Mux Dynamic Energy         = " << TO_JOULE(bank->mat.subarray.bitlineMux.readDynamicEnergy
													+ bank->mat.subarray.senseAmpMuxLev1.readDynamicEnergy
													+ bank->mat.subarray.senseAmpMuxLev2.readDynamicEnergy) << endl;
	cout << string(indent, ' ') << "       |--- Precharge Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.precharger.readDynamicEnergy) << endl;

	if (cell->memCellType == PCRAM || cell->memCellType == FBRAM || cell->memCellType == DWM || cell->memCellType == MRAM ||
			(cell->memCellType == memristor && (cell->accessType == CMOS_access || cell->accessType == BJT_access))) {
		if(cell->memCellType==PCRAM && cell->memCellLevel!=SLC){
			cout << string(indent, ' ') << " - Write Dynmamic Energy = " << TO_JOULE(bank->resetDynamicEnergy+bank->setDynamicEnergy) << endl;
			if (inputParameter->writeScheme==set_before_reset)
				cout << string(indent, ' ') << "  |--- Single Bit Write Energy = " <<TO_JOULE(cell->setEnergy+(cell->resetEnergy+cell->readPower*cell->readPulse)*cell->avgIterations)<<endl;
			else
				cout << string(indent, ' ') << "  |--- Single Bit Write Energy = " <<TO_JOULE(cell->resetEnergy+(cell->setEnergy+cell->readPower*cell->readPulse)*cell->avgIterations)<<endl;

		}


			cout << string(indent, ' ') << " - Write Dynamic Energy = " << TO_JOULE(bank->writeDynamicEnergy ) << endl;

		cout << string(indent, ' ') << " - RESET Dynamic Energy = " << TO_JOULE(bank->resetDynamicEnergy) << endl;
		if (cell->memCellType==MRAM && cell->memCellLevel!=SLC){
			cout << string(indent, ' ') << " - RESET Soft Dynamic Energy = " << TO_JOULE(bank->resetDynamicEnergySoft) << endl;
		}

        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            cout << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.resetDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numAccessBits) << endl;
           //cout<<"SEE1 bank->tsvArray.resetDynamicEnergy "<<bank->tsvArray.resetDynamicEnergy<<"\n";
	   //cout<<"SEE1 bank->tsvArray.numAccessBits "<<bank->tsvArray.numAccessBits<<"\n";
       	}
       	else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            cout << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.resetDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numAccessBits) << endl;
        }
		if (inputParameter->routingMode == h_tree)
			cout << string(indent, ' ') << " |--- H-Tree Dynamic Energy = " << TO_JOULE(bank->routingResetDynamicEnergy) << endl;
		else
			cout << string(indent, ' ') << " |--- H-Tree Dynamic Energy = " << TO_JOULE(bank->routingResetDynamicEnergy) << endl;
		cout << string(indent, ' ') << " |--- Mat Dynamic Energy    = " << TO_JOULE(bank->mat.resetDynamicEnergy) << " per mat" << endl;
		cout << string(indent, ' ') << "    |--- Predecoder Dynamic Energy = " << TO_JOULE(bank->mat.writeDynamicEnergy - bank->mat.subarray.writeDynamicEnergy
															* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
															<< endl;
		cout << string(indent, ' ') << "    |--- Subarray Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.writeDynamicEnergy) << " per active subarray" << endl;
		cout << string(indent, ' ') << "       |--- Row Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.rowDecoder.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << "       |--- Mux Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.bitlineMuxDecoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1Decoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2Decoder.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << "       |--- Mux Dynamic Energy         = " << TO_JOULE(bank->mat.subarray.bitlineMux.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << "       |--- Cell RESET Dynamic Energy  = " << TO_JOULE(bank->mat.subarray.cellResetEnergy)<<"  "<< TO_JOULE(bank->mat.subarray.cellResetEnergySoft)<< endl;
		cout << string(indent, ' ') << "       |--- Single Cell RESET Energy  = " << TO_JOULE(cell->resetEnergy)<<"  "<< TO_JOULE(cell->resetEnergySoft)  << endl;

		cout << string(indent, ' ') << " - SET Dynamic Energy = " << TO_JOULE(bank->setDynamicEnergy) << endl;
		if (cell->memCellType==MRAM && cell->memCellLevel!=SLC){
			cout << string(indent, ' ') << " - SET Soft Dynamic Energy = " << TO_JOULE(bank->setDynamicEnergySoft) << endl;
		}
		if (inputParameter->routingMode == h_tree)
			cout << string(indent, ' ') << " |--- H-Tree Dynamic Energy = " << TO_JOULE(bank->routingSetDynamicEnergy) << endl;
		else
			cout << string(indent, ' ') << " |--- Non-H-Tree Dynamic Energy = " << TO_JOULE(bank->routingSetDynamicEnergy) << endl;
		cout << string(indent, ' ') << " |--- Mat Dynamic Energy    = " << TO_JOULE(bank->mat.setDynamicEnergy) << " per mat" << endl;
		cout << string(indent, ' ') << "    |--- Predecoder Dynamic Energy = " << TO_JOULE(bank->mat.writeDynamicEnergy - bank->mat.subarray.writeDynamicEnergy
															* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
															<< endl;
		cout << string(indent, ' ') << "    |--- Subarray Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.writeDynamicEnergy) << " per active subarray" << endl;
		cout << string(indent, ' ') << "       |--- Row Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.rowDecoder.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << "       |--- Mux Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.bitlineMuxDecoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1Decoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2Decoder.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << "       |--- Mux Dynamic Energy         = " << TO_JOULE(bank->mat.subarray.bitlineMux.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << "       |--- Cell SET Dynamic Energy    = " << TO_JOULE(bank->mat.subarray.cellSetEnergy) <<"  "<< TO_JOULE(bank->mat.subarray.cellSetEnergySoft)<<"  "<<TO_JOULE(bank->mat.subarray.writeDynamicEnergy)<<endl;
		cout << string(indent, ' ') << "       |--- Single Cell SET Energy  = " << TO_JOULE(cell->setEnergy)<<"  "<< TO_JOULE(cell->setEnergySoft) << endl;

	} else if (cell->memCellType == SLCNAND||cell->memCellType==MLCNAND) {
		cout << string(indent, ' ') << " - Erase Dynamic Energy = " << TO_JOULE(bank->resetDynamicEnergy) << " per block" << endl;
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            cout << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.resetDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numAccessBits) << endl;
        //cout<<"SEE2 bank->tsvArray.resetDynamicEnergy "<<bank->tsvArray.resetDynamicEnergy<<"\n";
	//cout<<"SEE2 bank->tsvArray.numAccessBits "<<bank->tsvArray.numAccessBits<<"\n";
	} 
	else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            cout << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.resetDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numAccessBits) << endl;
        }
		if (inputParameter->routingMode == h_tree)
			cout << string(indent, ' ') << " |--- H-Tree Dynamic Energy = " << TO_JOULE(bank->routingResetDynamicEnergy) << endl;
		else
			cout << string(indent, ' ') << " |--- Non-H-Tree Dynamic Energy = " << TO_JOULE(bank->routingResetDynamicEnergy) << endl;
		cout << string(indent, ' ') << " |--- Mat Dynamic Energy    = " << TO_JOULE(bank->mat.resetDynamicEnergy) << " per mat" << endl;
		cout << string(indent, ' ') << "    |--- Predecoder Dynamic Energy = " << TO_JOULE(bank->mat.writeDynamicEnergy - bank->mat.subarray.writeDynamicEnergy
															* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
															<< endl;
		cout << string(indent, ' ') << "    |--- Subarray Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.writeDynamicEnergy) << " per active subarray" << endl;
		cout << string(indent, ' ') << "       |--- Row Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.rowDecoder.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << "       |--- Mux Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.bitlineMuxDecoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1Decoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2Decoder.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << "       |--- Mux Dynamic Energy         = " << TO_JOULE(bank->mat.subarray.bitlineMux.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << " - Programming Dynamic Energy = " << TO_JOULE(bank->setDynamicEnergy) << " per page" << endl;
		if (inputParameter->routingMode == h_tree)
			cout << string(indent, ' ') << " |--- H-Tree Dynamic Energy = " << TO_JOULE(bank->routingSetDynamicEnergy) << endl;
		else
			cout << string(indent, ' ') << " |--- Non-H-Tree Dynamic Energy = " << TO_JOULE(bank->routingSetDynamicEnergy) << endl;
		cout << string(indent, ' ') << " |--- Mat Dynamic Energy    = " << TO_JOULE(bank->mat.setDynamicEnergy) << " per mat" << endl;
		cout << string(indent, ' ') << "    |--- Predecoder Dynamic Energy = " << TO_JOULE(bank->mat.writeDynamicEnergy - bank->mat.subarray.writeDynamicEnergy
															* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
															<< endl;
		cout << string(indent, ' ') << "    |--- Subarray Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.writeDynamicEnergy) << " per active subarray" << endl;
		cout << string(indent, ' ') << "       |--- Row Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.rowDecoder.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << "       |--- Mux Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.bitlineMuxDecoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1Decoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2Decoder.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << "       |--- Mux Dynamic Energy         = " << TO_JOULE(bank->mat.subarray.bitlineMux.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2.writeDynamicEnergy) << endl;
	} else {
		cout << string(indent, ' ') << " - Write Dynamic Energy = " << TO_JOULE(bank->writeDynamicEnergy) << endl;
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            cout << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numAccessBits) << endl;
            //cout<<"SEE4 bank->tsvArray.resetDynamicEnergy "<<bank->tsvArray.resetDynamicEnergy<<"\n";
	    //cout<<"SEE4 bank->tsvArray.numAccessBits "<<bank->tsvArray.numAccessBits<<"\n";

	} else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            cout << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numAccessBits) << endl;
        }
		if (inputParameter->routingMode == h_tree)
			cout << string(indent, ' ') << " |--- H-Tree Dynamic Energy = " << TO_JOULE(bank->routingWriteDynamicEnergy) << endl;
		else
			cout << string(indent, ' ') << " |--- Non-H-Tree Dynamic Energy = " << TO_JOULE(bank->routingWriteDynamicEnergy) << endl;
		cout << string(indent, ' ') << " |--- Mat Dynamic Energy    = " << TO_JOULE(bank->mat.writeDynamicEnergy) << " per mat" << endl;
		cout << string(indent, ' ') << "    |--- Predecoder Dynamic Energy = " << TO_JOULE(bank->mat.writeDynamicEnergy - bank->mat.subarray.writeDynamicEnergy
															* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
															<< endl;
		cout << string(indent, ' ') << "    |--- Subarray Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.writeDynamicEnergy) << " per active subarray" << endl;
		cout << string(indent, ' ') << "       |--- Row Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.rowDecoder.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << "       |--- Mux Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.bitlineMuxDecoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1Decoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2Decoder.writeDynamicEnergy) << endl;
		cout << string(indent, ' ') << "       |--- Mux Dynamic Energy         = " << TO_JOULE(bank->mat.subarray.bitlineMux.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2.writeDynamicEnergy) << endl;
		if (cell->memCellType == MRAM) {
			cout << string(indent, ' ') << "       |--- Bitline & Cell Write Energy= " << TO_JOULE(bank->mat.subarray.cellResetEnergy) << endl;
		}
	}
    if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
        cout << string(indent, ' ') << " - Refresh Dynamic Energy = " << TO_JOULE(bank->refreshDynamicEnergy) << endl;
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            cout << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numReadBits) << endl;
        } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            cout << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numReadBits) << endl;
        }
		if (inputParameter->routingMode == h_tree)
			cout << string(indent, ' ') << " |--- H-Tree Dynamic Energy = " << TO_JOULE(bank->routingRefreshDynamicEnergy) << endl;
		else
			cout << string(indent, ' ') << " |--- Non-H-Tree Dynamic Energy = " << TO_JOULE(bank->routingRefreshDynamicEnergy) << endl;
        cout << string(indent, ' ') << " |--- Mat Dynamic Energy    = " << TO_JOULE(bank->mat.refreshDynamicEnergy) << " per mat" << endl;
        cout << string(indent, ' ') << "    |--- Predecoder Dynamic Energy = " << 
                TO_JOULE(bank->mat.refreshDynamicEnergy - bank->mat.subarray.refreshDynamicEnergy
                         * bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn) << endl;
        cout << string(indent, ' ') << "    |--- Subarray Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.refreshDynamicEnergy) << " per active subarray" << endl;
        cout << string(indent, ' ') << "       |--- Row Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.rowDecoder.refreshDynamicEnergy) << endl;
        if (inputParameter->internalSensing)
            cout << string(indent, ' ') << "       |--- Senseamp Dynamic Energy    = " << TO_JOULE(bank->mat.subarray.senseAmp.refreshDynamicEnergy) << endl;
        cout << string(indent, ' ') << "       |--- Precharge Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.precharger.refreshDynamicEnergy) << endl;
    }

	cout << string(indent, ' ') << " - Leakage Power = " << TO_WATT(bank->leakage) << endl;
	
    if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
        cout << string(indent, ' ') << " |--- TSV Leakage              = " << TO_WATT(bank->tsvArray.leakage * (bank->stackedDieCount-1) * bank->tsvArray.numTotalBits) << endl;
    } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
        cout << string(indent, ' ') << " |--- TSV Leakage              = " << TO_WATT(bank->tsvArray.leakage * (bank->stackedDieCount-1) * bank->tsvArray.numTotalBits + bank->mat.tsvArray.leakage * bank->numColumnMat * bank->numRowMat * bank->mat.tsvArray.numTotalBits) << endl;
    }
	if (inputParameter->routingMode == h_tree)
      {		cout << string(indent, ' ') << " |--- H-Tree Leakage Power     = " << TO_WATT(bank->routingLeakage) << endl;
          //cout<<"numRowMat "<<bank->numRowMat<<" numColumnMat "<<bank->numColumnMat<<"\n";
      }
	else
		cout << string(indent, ' ') << " |--- Non-H-Tree Leakage Power = " << TO_WATT(bank->routingLeakage) << endl;
	cout << string(indent, ' ') << " |--- Mat Leakage Power        = " << TO_WATT(bank->mat.leakage) << " per mat" << endl;
    if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
        cout << string(indent, ' ') << " - Refresh Power = " << TO_WATT(bank->refreshDynamicEnergy / (cell->retentionTime)) << endl;
    }
    if (cell->memCellType ==DWM) {
        cout << string(indent, ' ') << " - Shift Dynamic Energy = " << TO_JOULE(bank->shiftDynamicEnergy) << endl;
    }

	//OUR CORRECTION
	// Dynamic read power (J/B) readBandwidth * (readDynamicEnergy / per byte)
	double dynamic_read_power = readBandwidth * (bank->readDynamicEnergy / ((double)bank->blockSize / 8) );
	cout << string(indent, ' ') << " OUR - Read Dynamic Power = " << TO_WATT(dynamic_read_power) << endl;

	if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
        // address and control bit dynamics + read data output dynamic
		double tsv_dynamic_power = readBandwidth * ((bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numReadBits + bank->tsvArray.readDynamicEnergy * bank->tsvArray.numDataBits * (bank->stackedDieCount-1))/ 8);
        cout << string(indent, ' ') << " OUR - TSV Dynamic Power = " << TO_WATT(tsv_dynamic_power) << endl; //TSV Dynamic Power

	
    } 
    else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
        // address bit dynamic + control bit dynamic + read data dynamic
        // TODO: revisit this
		double tsv_dynamic_power = readBandwidth * ((bank->mat.tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * (bank->mat.tsvArray.numAccessBits) + bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->stackedDieCount + bank->tsvArray.readDynamicEnergy * bank->blockSize * (bank->stackedDieCount-1))/8);
        cout << string(indent, ' ') << " OUR - TSV Dynamic Power = " << TO_WATT(tsv_dynamic_power) << endl;//TSV Read Dynamic Energy
    }
}

void Result::printAsCache(Result &tagResult, CacheAccessMode cacheAccessMode) {
	if (bank->memoryType != data_type || tagResult.bank->memoryType != tag) {
		cout << "This is not a valid cache configuration." << endl;
		return;
	} else {
		double cacheHitLatency, cacheMissLatency, cacheWriteLatency;
		double cacheHitDynamicEnergy, cacheMissDynamicEnergy, cacheWriteDynamicEnergy;
		double cacheLeakage;
		double cacheArea;
		if (cacheAccessMode == normal_access_mode) {
			/* Calculate latencies */
			cacheMissLatency = tagResult.bank->readLatency;		/* only the tag access latency */
			cacheHitLatency = MAX(tagResult.bank->readLatency, bank->mat.readLatency);	/* access tag and activate data row in parallel */
			cacheHitLatency += bank->mat.subarray.columnDecoderLatency;		/* add column decoder latency after hit signal arrives */
			cacheHitLatency += bank->readLatency - bank->mat.readLatency;	/* H-tree in and out latency */
			cacheWriteLatency = MAX(tagResult.bank->writeLatency, bank->writeLatency);	/* Data and tag are written in parallel */
			/* Calculate power */
			cacheMissDynamicEnergy = tagResult.bank->readDynamicEnergy;	/* no matter what tag is always accessed */
			cacheMissDynamicEnergy += bank->readDynamicEnergy;	/* data is also partially accessed, TO-DO: not accurate here */
			cacheHitDynamicEnergy = tagResult.bank->readDynamicEnergy + bank->readDynamicEnergy;
			cacheWriteDynamicEnergy = tagResult.bank->writeDynamicEnergy + bank->writeDynamicEnergy;
		} else if (cacheAccessMode == fast_access_mode) {
			/* Calculate latencies */
			cacheMissLatency = tagResult.bank->readLatency;
			cacheHitLatency = MAX(tagResult.bank->readLatency, bank->readLatency);
			cacheWriteLatency = MAX(tagResult.bank->writeLatency, bank->writeLatency);
			/* Calculate power */
			cacheMissDynamicEnergy = tagResult.bank->readDynamicEnergy;	/* no matter what tag is always accessed */
			cacheMissDynamicEnergy += bank->readDynamicEnergy;	/* data is also partially accessed, TO-DO: not accurate here */
			cacheHitDynamicEnergy = tagResult.bank->readDynamicEnergy + bank->readDynamicEnergy;
			cacheWriteDynamicEnergy = tagResult.bank->writeDynamicEnergy + bank->writeDynamicEnergy;
		} else {		/* sequential access */
			/* Calculate latencies */
			cacheMissLatency = tagResult.bank->readLatency;
			cacheHitLatency = tagResult.bank->readLatency + bank->readLatency;
			//cacheWriteLatency = MAX(tagResult.bank->writeLatency, bank->writeLatency);
			cacheWriteLatency = tagResult.bank->writeLatency+ bank->writeLatency;
			/* Calculate power */
			cacheMissDynamicEnergy = tagResult.bank->readDynamicEnergy;	/* no matter what tag is always accessed */
			cacheHitDynamicEnergy = tagResult.bank->readDynamicEnergy + bank->readDynamicEnergy;
			cacheWriteDynamicEnergy = tagResult.bank->writeDynamicEnergy + bank->writeDynamicEnergy;
		}
		/* Calculate leakage */
		cacheLeakage = tagResult.bank->leakage + bank->leakage;
		/* Calculate area */
		cacheArea = tagResult.bank->area + bank->area;	/* TO-DO: simply add them together here */

		/* start printing */
		cout << endl << "=======================" << endl << "CACHE DESIGN -- SUMMARY" << endl << "=======================" << endl;
		cout << "Access Mode: ";
		switch (cacheAccessMode) {
		case normal_access_mode:
			cout << "Normal" << endl;
			break;
		case fast_access_mode:
			cout << "Fast" << endl;
			break;
		default:	/* sequential */
			cout << "Sequential" << endl;
		}
		cout << "Area:" << endl;
		cout << " - Total Area = " << cacheArea * 1e6 << "mm^2" << endl;
		cout << " |--- Data Array Area = " << bank->height * 1e6 << "um x " << bank->width * 1e6 << "um = " << bank->area * 1e6 << "mm^2" << endl;
		cout << " |--- Tag Array Area  = " << tagResult.bank->height * 1e6 << "um x " << tagResult.bank->width * 1e6 << "um = " << tagResult.bank->area * 1e6 << "mm^2" << endl;
		cout << "Timing:" << endl;
		cout << " - Cache Hit Latency   = " << cacheHitLatency * 1e9 << "ns" << endl;
		cout << " - Cache Miss Latency  = " << cacheMissLatency * 1e9 << "ns" << endl;
		cout << " - Cache Write Latency = " << cacheWriteLatency * 1e9 << "ns" << endl;
        if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
            cout << " - Cache Refresh Latency = " << MAX(tagResult.bank->refreshLatency, bank->refreshLatency) * 1e6 << "us per bank" << endl;
            cout << " - Cache Availability = " << ((cell->retentionTime - MAX(tagResult.bank->refreshLatency, bank->refreshLatency)) / cell->retentionTime) * 100.0 << "%" << endl;
        }
		cout << "Power:" << endl;
		cout << " - Cache Hit Dynamic Energy   = " << cacheHitDynamicEnergy * 1e9 << "nJ per access" << endl;
		cout << " - Cache Miss Dynamic Energy  = " << cacheMissDynamicEnergy * 1e9 << "nJ per access" << endl;
		cout << " - Cache Write Dynamic Energy = " << cacheWriteDynamicEnergy * 1e9 << "nJ per access" << endl;
		        if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
            cout << " - Cache Refresh Dynamic Energy = " << (tagResult.bank->refreshDynamicEnergy + bank->refreshDynamicEnergy) * 1e9 << "nJ per bank" << endl;
        }
		cout << " - Cache Total Leakage Power  = " << cacheLeakage * 1e3 << "mW" << endl;
		cout << " |--- Cache Data Array Leakage Power = " << bank->leakage * 1e3 << "mW" << endl;
		cout << " |--- Cache Tag Array Leakage Power  = " << tagResult.bank->leakage * 1e3 << "mW" << endl;
        if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
            cout << " - Cache Refresh Power = " << TO_WATT(bank->refreshDynamicEnergy / (cell->retentionTime)) << " per bank" << endl;
        }

            cout << endl << "CACHE DATA ARRAY DETAILS";
            print(4);
            cout << endl << "CACHE TAG ARRAY DETAILS";
            tagResult.print(4);

    	}


}

void Result::printAsCacheToCsvShort(Result &tagResult, CacheAccessMode cacheAccessMode, ofstream &outputFile) {
	if (bank->memoryType != data_type || tagResult.bank->memoryType != tag) {
		cout << "This is not a valid cache configuration." << endl;
		return;
	} else {
		double cacheHitLatency, cacheMissLatency, cacheWriteLatency;
		double cacheHitDynamicEnergy, cacheMissDynamicEnergy, cacheWriteDynamicEnergy;
		double cacheLeakage;
		double cacheArea;
		if (cacheAccessMode == normal_access_mode) {
			/* Calculate latencies */
			cacheMissLatency = tagResult.bank->readLatency;		/* only the tag access latency */
			cacheHitLatency = MAX(tagResult.bank->readLatency, bank->mat.readLatency);	/* access tag and activate data row in parallel */
			cacheHitLatency += bank->mat.subarray.columnDecoderLatency;		/* add column decoder latency after hit signal arrives */
			cacheHitLatency += bank->readLatency - bank->mat.readLatency;	/* H-tree in and out latency */
			cacheWriteLatency = MAX(tagResult.bank->writeLatency, bank->writeLatency);	/* Data and tag are written in parallel */
			/* Calculate power */
			cacheMissDynamicEnergy = tagResult.bank->readDynamicEnergy;	/* no matter what tag is always accessed */
			cacheMissDynamicEnergy += bank->readDynamicEnergy;	/* data is also partially accessed, TO-DO: not accurate here */
			cacheHitDynamicEnergy = tagResult.bank->readDynamicEnergy + bank->readDynamicEnergy;
			cacheWriteDynamicEnergy = tagResult.bank->writeDynamicEnergy + bank->writeDynamicEnergy;
		} else if (cacheAccessMode == fast_access_mode) {
			/* Calculate latencies */
			cacheMissLatency = tagResult.bank->readLatency;
			cacheHitLatency = MAX(tagResult.bank->readLatency, bank->readLatency);
			cacheWriteLatency = MAX(tagResult.bank->writeLatency, bank->writeLatency);
			/* Calculate power */
			cacheMissDynamicEnergy = tagResult.bank->readDynamicEnergy;	/* no matter what tag is always accessed */
			cacheMissDynamicEnergy += bank->readDynamicEnergy;	/* data is also partially accessed, TO-DO: not accurate here */
			cacheHitDynamicEnergy = tagResult.bank->readDynamicEnergy + bank->readDynamicEnergy;
			cacheWriteDynamicEnergy = tagResult.bank->writeDynamicEnergy + bank->writeDynamicEnergy;
		} else {		/* sequential access */
			/* Calculate latencies */
			cacheMissLatency = tagResult.bank->readLatency;
			cacheHitLatency = tagResult.bank->readLatency + bank->readLatency;
			cacheWriteLatency = tagResult.bank->writeLatency+bank->writeLatency;
			//cacheWriteLatency = MAX(tagResult.bank->writeLatency, bank->writeLatency);

			/* Calculate power */
			cacheMissDynamicEnergy = tagResult.bank->readDynamicEnergy;	/* no matter what tag is always accessed */
			cacheHitDynamicEnergy = tagResult.bank->readDynamicEnergy + bank->readDynamicEnergy;
			cacheWriteDynamicEnergy = tagResult.bank->writeDynamicEnergy + bank->writeDynamicEnergy;
		}
		/* Calculate leakage */
		cacheLeakage = tagResult.bank->leakage + bank->leakage;
		/* Calculate area */
		cacheArea = tagResult.bank->area + bank->area;	/* TO-DO: simply add them together here */


		/* start printing */
		outputFile << ToStringOptTarget(tagResult.optimizationTarget)<<",";

		outputFile << ToStringMemCellType(cell->memCellType)<<",";
		outputFile << cell->inputFileName<<",";

		switch (cacheAccessMode) {
		case normal_access_mode:
			outputFile << "Normal" << ",";
			break;
		case fast_access_mode:
			outputFile << "Fast" << ",";
			break;
		default:	 //sequential
			outputFile << "Sequential" << ",";
		}


		outputFile << cacheHitLatency * 1e9 << ",";
		outputFile << cacheMissLatency * 1e9 << ",";
		outputFile << cacheWriteLatency * 1e9 << ",";

		outputFile << cacheHitDynamicEnergy * 1e9 << ",";
		outputFile << cacheMissDynamicEnergy * 1e9 << ",";
		outputFile << cacheWriteDynamicEnergy * 1e9 << ",";

		outputFile << cacheHitDynamicEnergy*cacheHitLatency * 1e9* 1e9 << ",";
		outputFile << cacheWriteDynamicEnergy*cacheWriteLatency * 1e9* 1e9 << ",";

		outputFile << cacheLeakage * 1e3 << ",";
		outputFile << cacheArea * 1e6 << ",";
		//printToCsvFile(outputFile);
		//tagResult.printToCsvFile(outputFile);
		//outputFile << bank->mat.subarray.leakage + tagResult.bank->mat.subarray.leakage << ",";
		//outputFile << (bank->mat.subarray.area + tagResult.bank->mat.subarray.area) * 1e6 << ",";
		outputFile << endl;


	}
}

void Result::printAsCacheToCsvFile(Result &tagResult, CacheAccessMode cacheAccessMode, ofstream &outputFile) {
         //std::cout<<" comes to printAsCacheToCsvFile \n";
	if (bank->memoryType != data_type || tagResult.bank->memoryType != tag) {
		cout << "This is not a valid cache configuration." << endl;
		return;
	} else {
		double cacheHitLatency, cacheMissLatency, cacheWriteLatency;
		double cacheHitDynamicEnergy, cacheMissDynamicEnergy, cacheWriteDynamicEnergy;
		double cacheLeakage;
		double cacheArea;
		if (cacheAccessMode == normal_access_mode) {
			/* Calculate latencies */
			cacheMissLatency = tagResult.bank->readLatency;		/* only the tag access latency */
			cacheHitLatency = MAX(tagResult.bank->readLatency, bank->mat.readLatency);	/* access tag and activate data row in parallel */
			cacheHitLatency += bank->mat.subarray.columnDecoderLatency;		/* add column decoder latency after hit signal arrives */
			cacheHitLatency += bank->readLatency - bank->mat.readLatency;	/* H-tree in and out latency */
			cacheWriteLatency = MAX(tagResult.bank->writeLatency, bank->writeLatency);	/* Data and tag are written in parallel */
			/* Calculate power */
			cacheMissDynamicEnergy = tagResult.bank->readDynamicEnergy;	/* no matter what tag is always accessed */
			cacheMissDynamicEnergy += bank->readDynamicEnergy;	/* data is also partially accessed, TO-DO: not accurate here */
			cacheHitDynamicEnergy = tagResult.bank->readDynamicEnergy + bank->readDynamicEnergy;
			cacheWriteDynamicEnergy = tagResult.bank->writeDynamicEnergy + bank->writeDynamicEnergy;
		} else if (cacheAccessMode == fast_access_mode) {
			/* Calculate latencies */
			cacheMissLatency = tagResult.bank->readLatency;
			cacheHitLatency = MAX(tagResult.bank->readLatency, bank->readLatency);
			cacheWriteLatency = MAX(tagResult.bank->writeLatency, bank->writeLatency);
			/* Calculate power */
			cacheMissDynamicEnergy = tagResult.bank->readDynamicEnergy;	/* no matter what tag is always accessed */
			cacheMissDynamicEnergy += bank->readDynamicEnergy;	/* data is also partially accessed, TO-DO: not accurate here */
			cacheHitDynamicEnergy = tagResult.bank->readDynamicEnergy + bank->readDynamicEnergy;
			cacheWriteDynamicEnergy = tagResult.bank->writeDynamicEnergy + bank->writeDynamicEnergy;
		} else {		/* sequential access */
			/* Calculate latencies */
			cacheMissLatency = tagResult.bank->readLatency;
			cacheHitLatency = tagResult.bank->readLatency + bank->readLatency;
			cacheWriteLatency = tagResult.bank->writeLatency+bank->writeLatency;
			//cacheWriteLatency = MAX(tagResult.bank->writeLatency, bank->writeLatency);

			/* Calculate power */
			cacheMissDynamicEnergy = tagResult.bank->readDynamicEnergy;	/* no matter what tag is always accessed */
			cacheHitDynamicEnergy = tagResult.bank->readDynamicEnergy + bank->readDynamicEnergy;
			cacheWriteDynamicEnergy = tagResult.bank->writeDynamicEnergy + bank->writeDynamicEnergy;
		}
		/* Calculate leakage */
		cacheLeakage = tagResult.bank->leakage + bank->leakage;
		/* Calculate area */
		cacheArea = tagResult.bank->area + bank->area;	/* TO-DO: simply add them together here */

	   	/* start printing */
	   if(inputParameter->isPruningEnabled==true&&inputParameter->optimizationTarget == full_exploration){
		    outputFile << cell->memCellType<<",";

			switch (cacheAccessMode) {
			case normal_access_mode:
				outputFile << "Normal" << ",";
				break;
			case fast_access_mode:
				outputFile << "Fast" << ",";
				break;
			default:	 //sequential
				outputFile << "Sequential" << ",";
			}
			outputFile << cacheArea * 1e6 << ",";
			outputFile << cacheHitLatency * 1e9 << ",";
			outputFile << cacheMissLatency * 1e9 << ",";
			outputFile << cacheWriteLatency * 1e9 << ",";
			if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
				outputFile << MAX(tagResult.bank->refreshLatency, bank->refreshLatency) * 1e9 << ",";
			} else {
				outputFile << "0,";
			}
			outputFile << cacheHitDynamicEnergy * 1e9 << ",";
			outputFile << cacheMissDynamicEnergy * 1e9 << ",";
			outputFile << cacheWriteDynamicEnergy * 1e9 << ",";
			if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
				outputFile << (tagResult.bank->refreshDynamicEnergy + bank->refreshDynamicEnergy) * 1e9 << ",";
			} else {
				outputFile << "0,";
			}
			outputFile << cacheLeakage * 1e3 << ",";
			if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
				outputFile << TO_WATT(bank->refreshDynamicEnergy / (cell->retentionTime)) << ",";
			} else {
				outputFile << "0,";
			}
			printToCsvFile(outputFile);
			tagResult.printToCsvFile(outputFile);
			outputFile << bank->mat.subarray.leakage + tagResult.bank->mat.subarray.leakage << ",";
			outputFile << (bank->mat.subarray.area + tagResult.bank->mat.subarray.area) * 1e6 << ",";
			outputFile << endl;

	   }
	}


}


void Result::printToCsvFile(ofstream &outputFile) {
    
    
    outputFile << cell->memCellType<<",";
	outputFile << bank->readDynamicEnergy * 1e12 << ",";
	outputFile << (bank->readDynamicEnergy - bank->mat.readDynamicEnergy
					* bank->numActiveMatPerColumn * bank->numActiveMatPerRow)
					* 1e12 << ",";
	outputFile << bank->mat.readDynamicEnergy * 1e12 << ",";
	outputFile << (bank->mat.readDynamicEnergy - bank->mat.subarray.readDynamicEnergy
					* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
					* 1e12 << ",";
	outputFile << bank->mat.subarray.readDynamicEnergy * 1e12 << ",";
	outputFile << bank->mat.subarray.rowDecoder.readDynamicEnergy * 1e12 << ",";
	outputFile << (bank->mat.subarray.bitlineMuxDecoder.readDynamicEnergy
					+ bank->mat.subarray.senseAmpMuxLev1Decoder.readDynamicEnergy
					+ bank->mat.subarray.senseAmpMuxLev2Decoder.readDynamicEnergy) * 1e12 << ",";
	outputFile << bank->mat.subarray.senseAmp.readDynamicEnergy * 1e12 << ",";
	outputFile << (bank->mat.subarray.bitlineMux.readDynamicEnergy
					+ bank->mat.subarray.senseAmpMuxLev1.readDynamicEnergy
					+ bank->mat.subarray.senseAmpMuxLev2.readDynamicEnergy) * 1e12 << ",";
	outputFile << bank->mat.subarray.precharger.readDynamicEnergy * 1e12 << ",";
	outputFile << bank->writeDynamicEnergy * 1e12 << ",";
	outputFile << (bank->writeDynamicEnergy - bank->mat.writeDynamicEnergy
					* bank->numActiveMatPerColumn * bank->numActiveMatPerRow)
					* 1e12 << ",";
	outputFile << bank->mat.writeDynamicEnergy * 1e12 << ",";
	outputFile << (bank->mat.writeDynamicEnergy - bank->mat.subarray.writeDynamicEnergy
					* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
					* 1e12 << ",";
	outputFile << bank->mat.subarray.writeDynamicEnergy * 1e12 << ",";
	outputFile << bank->mat.subarray.rowDecoder.writeDynamicEnergy * 1e12 << ",";
	outputFile << (bank->mat.subarray.bitlineMuxDecoder.writeDynamicEnergy
					+ bank->mat.subarray.senseAmpMuxLev1Decoder.writeDynamicEnergy
					+ bank->mat.subarray.senseAmpMuxLev2Decoder.writeDynamicEnergy) * 1e12 << ",";
	outputFile << (bank->mat.subarray.bitlineMux.writeDynamicEnergy
					+ bank->mat.subarray.senseAmpMuxLev1.writeDynamicEnergy
					+ bank->mat.subarray.senseAmpMuxLev2.writeDynamicEnergy) * 1e12 << endl;

	outputFile << bank->numRowMat << "," << bank->numColumnMat << "," << bank->stackedDieCount << "," << bank->numActiveMatPerColumn << "," << bank->numActiveMatPerRow << ",";
	outputFile << bank->numRowSubarray << "," << bank->numColumnSubarray << "," << bank->numActiveSubarrayPerColumn << "," << bank->numActiveSubarrayPerRow << ",";
	outputFile << bank->mat.subarray.numRow << "," << bank->mat.subarray.numColumn << ",";
	outputFile << bank->muxSenseAmp << "," << bank->muxOutputLev1 << "," << bank->muxOutputLev2 << ",";
	if (inputParameter->designTarget == cache)
		outputFile << bank->numRowPerSet << ",";
	else
		outputFile << "N/A,";
	switch (localWire->wireType) {
	case local_aggressive:
		outputFile << "Local Aggressive" << ",";
		break;
	case local_conservative:
		outputFile << "Local Conservative" << ",";
		break;
	case semi_aggressive:
		outputFile << "Semi-Global Aggressive" << ",";
		break;
	case semi_conservative:
		outputFile << "Semi-Global Conservative" << ",";
		break;
	case global_aggressive:
		outputFile << "Global Aggressive" << ",";
		break;
	case global_conservative:
		outputFile << "Global Conservative" << ",";
		break;
	default:
		outputFile << "DRAM Wire" << ",";
	}
	switch (localWire->wireRepeaterType) {
	case repeated_none:
		outputFile << "No Repeaters" << ",";
		break;
	case repeated_opt:
		outputFile << "Fully-Optimized Repeaters" << ",";
		break;
	case repeated_5:
		outputFile << "Repeaters with 5% Overhead" << ",";
		break;
	case repeated_10:
		outputFile << "Repeaters with 10% Overhead" << ",";
		break;
	case repeated_20:
		outputFile << "Repeaters with 20% Overhead" << ",";
		break;
	case repeated_30:
		outputFile << "Repeaters with 30% Overhead" << ",";
		break;
	case repeated_40:
		outputFile << "Repeaters with 40% Overhead" << ",";
		break;
	case repeated_50:
		outputFile << "Repeaters with 50% Overhead" << ",";
		break;
	default:
		outputFile << "N/A" << ",";
	}
	if (localWire->isLowSwing)
		outputFile << "Yes" << ",";
	else
		outputFile << "No" << ",";
	switch (globalWire->wireType) {
	case local_aggressive:
		outputFile << "Local Aggressive" << ",";
		break;
	case local_conservative:
		outputFile << "Local Conservative" << ",";
		break;
	case semi_aggressive:
		outputFile << "Semi-Global Aggressive" << ",";
		break;
	case semi_conservative:
		outputFile << "Semi-Global Conservative" << ",";
		break;
	case global_aggressive:
		outputFile << "Global Aggressive" << ",";
		break;
	case global_conservative:
		outputFile << "Global Conservative" << ",";
		break;
	default:
		outputFile << "DRAM Wire" << ",";
	}
	switch (globalWire->wireRepeaterType) {
	case repeated_none:
		outputFile << "No Repeaters" << ",";
		break;
	case repeated_opt:
		outputFile << "Fully-Optimized Repeaters" << ",";
		break;
	case repeated_5:
		outputFile << "Repeaters with 5% Overhead" << ",";
		break;
	case repeated_10:
		outputFile << "Repeaters with 10% Overhead" << ",";
		break;
	case repeated_20:
		outputFile << "Repeaters with 20% Overhead" << ",";
		break;
	case repeated_30:
		outputFile << "Repeaters with 30% Overhead" << ",";
		break;
	case repeated_40:
		outputFile << "Repeaters with 40% Overhead" << ",";
		break;
	case repeated_50:
		outputFile << "Repeaters with 50% Overhead" << ",";
		break;
	default:
		outputFile << "N/A" << ",";
	}
	if (globalWire->isLowSwing)
		outputFile << "Yes" << ",";
	else
		outputFile << "No" << ",";
	switch (bank->areaOptimizationLevel) {
	case latency_first:
		outputFile << "Latency-Optimized" << ",";
		break;
	case area_first:
		outputFile << "Area-Optimized" << ",";
		break;
	default:	/* balance */
		outputFile << "Balanced" << ",";
	}
	outputFile << bank->height * 1e6 << "," << bank->width * 1e6 << "," << bank->area * 1e6 << ",";
	outputFile << bank->mat.height * 1e6 << "," << bank->mat.width * 1e6 << "," << bank->mat.area * 1e6 << ",";
	outputFile << bank->mat.subarray.height * 1e6 << "," << bank->mat.subarray.width * 1e6 << "," << bank->mat.subarray.area * 1e6 << ",";
	outputFile << cell->area * tech->featureSize * tech->featureSize * bank->capacity / bank->area * 100 << ",";
	outputFile << bank->readLatency * 1e9 << "," << bank->writeLatency * 1e9 << ",";
    if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
        outputFile << bank->refreshLatency * 1e9 << ",";
    } else {
        outputFile << "0,";
    }
	outputFile << bank->readDynamicEnergy * 1e12 << "," << bank->writeDynamicEnergy * 1e12 << ",";
    if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
        outputFile << bank->refreshDynamicEnergy * 1e12 << ",";
    } else {
        outputFile << "0,";
    }
	outputFile << bank->leakage * 1e3 << ",";
    if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
        outputFile << TO_WATT(bank->refreshDynamicEnergy / (cell->retentionTime)) << ",";
    } else {
        outputFile << "0,";
    }
}



void Result::print_to_csv(ofstream &outputFile, int indent) {
	//Cell Technology,Cell Area (F^2),Cell Aspect Ratio,Cell Turned-On Resistance,Cell Turned-Off Resistance,Read Mode,Read Current,Reset Mode,Reset Current,Reset Pulse,Set Mode,Set Current,Set Pulse,Access Type
	//HERE ADD COLUMNS WHEN YOU WANT SOME MORE INFO!
	outputFile<< "Optimization Target"
			<< ",Bank Organization,Bank Row Activation,Bank Column Activation,Mat Organization,Mat Row Activation,Mat Column Activation,Subarray Size,Senseamp Mux,Output Level-1 Mux,Output Level-2 Mux"
			<< ",Local Wire Type,Local Wire Repeater Type,Local Wire Low Swing,Global Wire Type,Global Wire Repeater Type,Global Wire Low Swing,Buffer Design Style"
			<< ",Height Total,Width Total,Total Area,Height Mat,Width Mat,Mat Area,Mat Efficiency,Height Subarray,Width Subarry,Subarray Area,Subarray Efficiency"
			<< ",TSV Area,Logic Layer Area,Area Efficiency"
			<< ",Read Latency,TSV Read Latency,H-Tree Read Latency,Bus Read Latency,Mat Read Latency,Predecoder Read Latency,Subarray Read Latency,Row Read Decoder Latency,Bitline Read Latency (ns:ns:ns),Senseamp read Latency,Muxread Latency,Precharge read Latency,Read Pulse,Comparator read Latency"
			<< ",Write Total Latency,RESET Latency,RESET Soft Latency,SET Latency,SET Soft Latency"
			<< ",Read Bandwidth,Read Bandwidth per mat,Write Bandwidth"
			<< ",Read Dynamic Energy,TSV Read Dynamic Energy,H-tree Read Dynamic Energy,Non-H-Tree Read Dynamic Energy,Mat Read Dynamic Energy,Predecoder Read Dynamic Energy,Subarray Read Dynamic Energy,Row Decoder Read Dynamic Energy,Mux Decoder Read Dynamic Energy,Cell Read Dynamic Energy,Senseamp Read Dynamic Energy,Mux Read Dynamic Energy,Precharge Read Dynamic Energy"
			<< ",Set+Reset Dynamic Energy,Write Dynamic Energy,RESET Dynamic Energy,RESET Soft Dynamic Energy"
			<< ",TSV Reset Dynamic Energy, H-Tree Reset Dynamic Energy,BUS Reset Dynamic Energy, Mat Reset Dynamic Energy, Predecoder Reset Dynamic Energy"
			<< ",Subarray Read Dynamic Energy,Row Decoder Read Dynamic Energy,Mux Decoder Read Dynamic Energy,Mux Dynamic Energy,Cell RESET Read Dynamic Energy,Single Cell RESET Read Energy"
			<< ",Set Dynamic Energy, Set Soft Dynamic Energy,H-Tree Set Dynamic Energy,Bus Set Dynamic Energy,Mat Set Dynamic Energy,Predecoder Dynamic Energy"
			<< ",Subarray Set Dynamic Energy, Row Decoder Set Dynamic Energy, Mux Decoder Set Dynamic Energy, Mux Set Dynamic Energy, Cell SET Dynamic Energy,Single Cell SET Energy "
			<< ",Leakage Power,TSV Leakage Power,H-Tree Leakage Power,Bus Leakage Power,Mat Leakage Power per mat,Read Dynamic Power,TSV Dynamic Power"
			<< endl;

	outputFile << string(indent, ' ') << printOptimizationTarget()<<","; //Optimization Target
	//cellTech->PrintCell(indent); TO DO columny są juz gotowe //Cell
	if (bank->stackedDieCount >1) { 

		outputFile << string(indent, ' ') << bank->numRowMat << " x " << bank->numColumnMat << " x " << bank->stackedDieCount<<",";//Bank Organization
              outputFile << string(indent, ' ') << bank->numActiveMatPerColumn << " / " << bank->numRowMat << " x 1"<<",";//Row Activation
              outputFile << string(indent, ' ') << bank->numActiveMatPerRow << " / " << bank->numColumnMat << " x 1"<<","; //Column Activation
       } else {
              outputFile << string(indent, ' ') << bank->numRowMat << " x " << bank->numColumnMat<<","; //Mat Organization
              outputFile << string(indent, ' ') << bank->numActiveMatPerColumn << " / " << bank->numRowMat<<","; //Row Activation
              outputFile << string(indent, ' ')  << bank->numActiveMatPerRow << " / " << bank->numColumnMat<<","; //Column Activation
       }

	outputFile << string(indent, ' ') << bank->numRowSubarray << " x " << bank->numColumnSubarray<<","; //Subarray Organization
	outputFile << string(indent, ' ') << bank->numActiveSubarrayPerColumn << " / " << bank->numRowSubarray<<","; //Row Activation
	outputFile << string(indent, ' ') << bank->numActiveSubarrayPerRow << " / " << bank->numColumnSubarray<<","; //Column Activation
	outputFile << string(indent, ' ') << bank->mat.subarray.numRow << " Rows x " << bank->mat.subarray.numColumn << " Columns"<<","; //Subarray Size
	outputFile << string(indent, ' ') << bank->muxSenseAmp<<","; //Senseamp Mux
	outputFile << string(indent, ' ') << bank->muxOutputLev1<<","; //Output Level-1 Mux
	outputFile << string(indent, ' ') << bank->muxOutputLev2<<","; //Output Level-2 Mux

	switch (localWire->wireType) { //Local Wire Type
	case local_aggressive:
		outputFile << string(indent, ' ') << "Local Aggressive" << ",";
		break;
	case local_conservative:
		outputFile << string(indent, ' ') << "Local Conservative" << ",";
		break;
	case semi_aggressive:
		outputFile << string(indent, ' ') << "Semi-Global Aggressive" << ",";
		break;
	case semi_conservative:
		outputFile << string(indent, ' ') << "Semi-Global Conservative" << ",";
		break;
	case global_aggressive:
		outputFile << string(indent, ' ') << "Global Aggressive" << ",";
		break;
	case global_conservative:
		outputFile << string(indent, ' ') << "Global Conservative" << ",";
		break;
	default:
		outputFile << string(indent, ' ') << "DRAM Wire" << ",";
	}; 

	switch (localWire->wireRepeaterType) { //Local Wire Repeater Type
	case repeated_none:
		outputFile << string(indent, ' ') << "No Repeaters" << ",";
		break;
	case repeated_opt:
		outputFile << string(indent, ' ') << "Fully-Optimized Repeaters" << ",";
		break;
	case repeated_5:
		outputFile << string(indent, ' ') << "Repeaters with 5% Overhead" << ",";
		break;
	case repeated_10:
		outputFile << string(indent, ' ') << "Repeaters with 10% Overhead" << ",";
		break;
	case repeated_20:
		outputFile << string(indent, ' ') << "Repeaters with 20% Overhead" << ",";
		break;
	case repeated_30:
		outputFile << string(indent, ' ') << "Repeaters with 30% Overhead" << ",";
		break;
	case repeated_40:
		outputFile << string(indent, ' ') << "Repeaters with 40% Overhead" << ",";
		break;
	case repeated_50:
		outputFile << string(indent, ' ') << "Repeaters with 50% Overhead" << ",";
		break;
	default:
		outputFile << string(indent, ' ') << "Unknown" << ",";
	}

	if (localWire->isLowSwing) //Local Wire Low Swing
		outputFile << string(indent, ' ') << "Yes" << ",";
	else
		outputFile << string(indent, ' ') << "No" << ",";


	switch (globalWire->wireType) { //Global Wire Type
	case local_aggressive:
		outputFile << string(indent, ' ') << "Local Aggressive" << ",";
		break;
	case local_conservative:
		outputFile << string(indent, ' ') << "Local Conservative" << ",";
		break;
	case semi_aggressive:
		outputFile << string(indent, ' ') << "Semi-Global Aggressive" << ",";
		break;
	case semi_conservative:
		outputFile << string(indent, ' ') << "Semi-Global Conservative" << ",";
		break;
	case global_aggressive:
		outputFile << string(indent, ' ') << "Global Aggressive" << ",";
		break;
	case global_conservative:
		outputFile << string(indent, ' ') << "Global Conservative" << ",";
		break;
	default:
		outputFile << string(indent, ' ') << "DRAM Wire" << ",";
	}

	switch (globalWire->wireRepeaterType) { //Global Wire Repeater
	case repeated_none:
		outputFile << string(indent, ' ') << "No Repeaters" << ",";
		break;
	case repeated_opt:
		outputFile << string(indent, ' ') << "Fully-Optimized Repeaters" << ",";
		break;
	case repeated_5:
		outputFile << string(indent, ' ') << "Repeaters with 5% Overhead" << ",";
		break;
	case repeated_10:
		outputFile << string(indent, ' ') << "Repeaters with 10% Overhead" << ",";
		break;
	case repeated_20:
		outputFile << string(indent, ' ') << "Repeaters with 20% Overhead" << ",";
		break;
	case repeated_30:
		outputFile << string(indent, ' ') << "Repeaters with 30% Overhead" << ",";
		break;
	case repeated_40:
		outputFile << string(indent, ' ') << "Repeaters with 40% Overhead" << ",";
		break;
	case repeated_50:
		outputFile << string(indent, ' ') << "Repeaters with 50% Overhead" << ",";
		break;
	default:
		outputFile << string(indent, ' ') << "Unknown" << ",";
	}

	if (globalWire->isLowSwing) //Global Wire Low Swing
		outputFile << string(indent, ' ') << "Yes" << ",";
	else
		outputFile << string(indent, ' ') << "No" << ",";



	switch (bank->areaOptimizationLevel) { //Buffer Design Style
	case latency_first:
		outputFile << string(indent, ' ') << "Latency-Optimized" << ",";
		break;
	case area_first:
		outputFile << string(indent, ' ') << "Area-Optimized" << ",";
		break;
	default:	/* balance */
		outputFile << string(indent, ' ') << "Balanced" << ",";
	}

	outputFile << string(indent, ' ') << TO_METER(bank->height) << ","; //Height Total
	outputFile << string(indent, ' ') << TO_METER(bank->width) << ","; //Width Total
	outputFile << string(indent, ' ') << TO_SQM(bank->area) << ","; //Total Area

	outputFile << string(indent, ' ') << TO_METER(bank->mat.height) << ","; //Height Mat
	outputFile << string(indent, ' ') << TO_METER(bank->mat.width) << ","; //Width Mat
	outputFile << string(indent, ' ') << TO_SQM(bank->mat.area) << ","; //Mat Area

	outputFile << string(indent, ' ') << cell->area * tech->featureSize * tech->featureSize
			* bank->capacity / bank->numRowMat / bank->numColumnMat / bank->mat.area * 100 << "%" << ","; //Mat Efficiency

	outputFile << string(indent, ' ') << TO_METER(bank->mat.subarray.height) << ","; //Height Subarray

	outputFile << string(indent, ' ') << TO_METER(bank->mat.subarray.width) << ","; //Width Subarry
	outputFile << string(indent, ' ') << TO_SQM(bank->mat.subarray.area) << ","; //Subarray Area
	
	outputFile << string(indent, ' ') << cell->area * tech->featureSize * tech->featureSize * bank->capacity / bank->numRowMat
			/ bank->numColumnMat / bank->numRowSubarray / bank->numColumnSubarray
			/ bank->mat.subarray.area * 100 << "%" <<","; //Subarray Efficiency

    if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
        outputFile << string(indent, ' ') << TO_SQM(bank->tsvArray.area) << ","; //TSV Area 
		outputFile << string(indent, ' ') << "N/A" << ","; //Logic Layer Area
    } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
        double totalTSVArea = bank->tsvArray.area + bank->mat.tsvArray.area
                              * bank->numColumnMat * bank->numRowMat;
        double areaLogicLayer = bank->mat.areaAllLogicBlocks * bank->numColumnMat
                              * bank->numRowMat;

        outputFile << string(indent, ' ') << TO_SQM(totalTSVArea) << ","; //TSV Area
        outputFile << string(indent, ' ') << TO_SQM(areaLogicLayer) << ","; //Logic Layer Area
    }
	else {
		outputFile << string(indent, ' ') << "N/A" << ","; //TSV Area
		outputFile << string(indent, ' ') << "N/A" << ","; //Logic Layer Area
	}


	outputFile << string(indent, ' ') << cell->area * tech->featureSize * tech->featureSize
			* bank->capacity / bank->area * 100 << "%" << ","; //Area Efficiency

	//Timing 
	outputFile << string(indent, ' ') << TO_SECOND(bank->readLatency) << ","; //Read Latency (ns)
    if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) { 
        double totalTSVLatency = (bank->tsvArray.readLatency + bank->tsvArray.writeLatency) * (bank->stackedDieCount-1);
        outputFile << string(indent, ' ') << TO_SECOND(totalTSVLatency) << ","; //TSV Latency (ns)
    } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
        double totalTSVLatency = bank->tsvArray.readLatency * (bank->stackedDieCount-1)
                               + bank->mat.tsvArray.writeLatency * (bank->stackedDieCount-1);

        outputFile << string(indent, ' ') << TO_SECOND(totalTSVLatency) << ","; //TSV Latency (ns)
    }
	else 
		outputFile << string(indent, ' ') << "N/A" << ",";

	if (inputParameter->routingMode == h_tree) {
		outputFile << string(indent, ' ') << TO_SECOND(bank->routingReadLatency) << ","; //H-Tree Latency (ns)
		outputFile << string(indent, ' ') << "N/A" << ",";
	}
	else {
		outputFile << string(indent, ' ') << "N/A" << ",";
		outputFile << string(indent, ' ') << TO_SECOND(bank->routingReadLatency) << ","; //Bus Latency (ns)
		}

	outputFile << string(indent, ' ') << TO_SECOND(bank->mat.readLatency) << ","; //Mat read Latency (ns)
	outputFile << string(indent, ' ') << TO_SECOND(bank->mat.predecoderLatency) << ","; //Predecoder read Latency (ns)
	outputFile << string(indent, ' ') << TO_SECOND(bank->mat.subarray.readLatency) << ","; //Subarray read Latency (ns),
	outputFile << string(indent, ' ') << TO_SECOND(bank->mat.subarray.rowDecoder.readLatency) << ","; //Row Decoder read Latency (ns)
	outputFile << string(indent, ' ') << TO_SECOND(bank->mat.subarray.bitlineDelay) << ":"<<TO_SECOND(bank->mat.subarray.bitlineDelayOn)<<":"<< TO_SECOND(bank->mat.subarray.bitlineDelayOff)<< ","; //Bitline read Latency (ns,ns,ns)
	
	if (inputParameter->internalSensing) //Senseamp read Latency (ns)
		outputFile << string(indent, ' ') << TO_SECOND(bank->mat.subarray.senseAmp.readLatency) << ",";
	else 
		outputFile << string(indent, ' ') << "N/A" << ",";

	outputFile << string(indent, ' ') << TO_SECOND(bank->mat.subarray.bitlineMux.readLatency 
													+ bank->mat.subarray.senseAmpMuxLev1.readLatency
													+ bank->mat.subarray.senseAmpMuxLev2.readLatency) << ","; //Mux read Latency (ns)
	outputFile << string(indent, ' ') << TO_SECOND(bank->mat.subarray.precharger.readLatency) << ","; //Precharge read Latency (ns)
	outputFile << string(indent, ' ') << TO_SECOND(cell->readPulse) << ","; //Read Pulse (ns)

	if (bank->mat.memoryType == tag && bank->mat.internalSenseAmp)
		outputFile << string(indent, ' ') << TO_SECOND(bank->mat.comparator.readLatency) << ","; //comparator read latency (ns)
	else 
		outputFile << string(indent, ' ') << "N/A" << ",";

	
	if (cell->memCellType == PCRAM || cell->memCellType == FBRAM || cell->memCellType ==MRAM ||
			(cell->memCellType == memristor && (cell->accessType == CMOS_access || cell->accessType == BJT_access || cell->memCellLevel!=SLC))) {
		outputFile << string(indent, ' ')  << TO_SECOND(bank->writeLatency) << ","; //Write Total Latency (ns)
		outputFile << string(indent, ' ')  << TO_SECOND(bank->resetLatency) << ","; //RESET write Latency (ns)
		if (cell->memCellType==MRAM && cell->memCellLevel!=SLC){
			outputFile << string(indent, ' ') << TO_SECOND(bank->resetLatencySoft) << ","; //RESET Soft Latency (ns)
		}
		else {
			outputFile << string(indent, ' ') << "N/A" << ","; //in not MRAM with MLC
		}
		/* Uncomment to get rest RESET Latency! and add columns up
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            outputFile << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.resetLatency * (bank->stackedDieCount-1)) << ",";
        } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            outputFile << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.resetLatency * (bank->stackedDieCount-1)) << ",";
        }
		if (inputParameter->routingMode == h_tree)
			outputFile << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingResetLatency) << ",";
		else
			outputFile << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingResetLatency) << ",";


		outputFile << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.resetLatency) << ",";
		outputFile << string(indent, ' ') << "    |--- Predecoder Latency = " << TO_SECOND(bank->mat.predecoderLatency) << ",";
		outputFile << string(indent, ' ') << "    |--- Subarray Latency   = " << TO_SECOND(bank->mat.subarray.resetLatency) << ",";
		outputFile << string(indent, ' ') << "       |--- RESET Pulse Duration = " << TO_SECOND(cell->resetPulse) << ",";
		if (cell->memCellType==MRAM && cell->memCellLevel!=SLC){
			outputFile << string(indent, ' ') << "       |--- RESET Soft Pulse Duration   = " << TO_SECOND(cell->resetPulseSoft) << ",";
		}
		outputFile << string(indent, ' ') << "       |--- Row Decoder Latency  = " << TO_SECOND(bank->mat.subarray.rowDecoder.writeLatency) << ",";
		outputFile << string(indent, ' ') << "       |--- Charge Latency   = " << TO_SECOND(bank->mat.subarray.chargeLatency) << ",";
		*/

		outputFile << string(indent, ' ') << TO_SECOND(bank->setLatency) << ","; //SET Latency (ns)
		if (cell->memCellType==MRAM && cell->memCellLevel!=SLC){
			outputFile << string(indent, ' ') << TO_SECOND(bank->setLatencySoft) << ","; //SET Soft Latency (ns)
		}
		else { 
			outputFile << string(indent, ' ') << "N/A"<<","; //if not MRAM with MLC
		}

		/*uncomment to get 3D of SET Latency details: 
		if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            outputFile << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.setLatency * (bank->stackedDieCount-1)) << ",";
        } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            outputFile << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.setLatency * (bank->stackedDieCount-1)) << ",";
        }
		if (inputParameter->routingMode == h_tree)
			outputFile << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingSetLatency) << ",";
		else
			outputFile << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingSetLatency) << ",";
		outputFile << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.setLatency) << ",";
		outputFile << string(indent, ' ') << "    |--- Predecoder Latency = " << TO_SECOND(bank->mat.predecoderLatency) << ",";
		outputFile << string(indent, ' ') << "    |--- Subarray Latency   = " << TO_SECOND(bank->mat.subarray.setLatency) << ",";
		outputFile << string(indent, ' ') << "       |--- SET Pulse Duration   = " << TO_SECOND(cell->setPulse) << ",";
		if (cell->memCellType==MRAM && cell->memCellLevel!=SLC){
			outputFile << string(indent, ' ') << "       |--- SET Soft Pulse Duration   = " << TO_SECOND(cell->setPulseSoft) << ",";
		}
		outputFile << string(indent, ' ') << "       |--- Row Decoder Latency  = " << TO_SECOND(bank->mat.subarray.rowDecoder.writeLatency) << ",";
		outputFile << string(indent, ' ') << "       |--- Charger Latency      = " << TO_SECOND(bank->mat.subarray.chargeLatency) << ",";

	//Uncomment this if you are working with NAND
	} else if (cell->memCellType == SLCNAND ||cell->memCellType ==MLCNAND) {
		outputFile << string(indent, ' ') << " - Erase Latency = " << TO_SECOND(bank->resetLatency) << ",";
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            outputFile << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.resetLatency * (bank->stackedDieCount-1)) << ",";
        } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            outputFile << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.resetLatency * (bank->stackedDieCount-1)) << ",";
        }
		if (inputParameter->routingMode == h_tree)
			outputFile << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingResetLatency) << ",";
		else
			outputFile << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingResetLatency) << ",";
		outputFile << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.resetLatency) << ",";
		outputFile << string(indent, ' ') << " - Programming Latency   = " << TO_SECOND(bank->setLatency) << ",";
		if (inputParameter->routingMode == h_tree)
			outputFile << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingSetLatency) << ",";
		else
			outputFile << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingSetLatency) << ",";
		outputFile << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.setLatency) << ",";
	} else {
		outputFile << string(indent, ' ') << " - Write Latency = " << TO_SECOND(bank->writeLatency) << ",";
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            outputFile << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.writeLatency * (bank->stackedDieCount-1)) << ",";
        } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            outputFile << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.writeLatency * (bank->stackedDieCount-1)) << ",";
        }
		if (inputParameter->routingMode == h_tree)
			outputFile << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingWriteLatency) << ",";
		else
			outputFile << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingWriteLatency) << ",";
		outputFile << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.writeLatency) << ",";
		outputFile << string(indent, ' ') << "    |--- Predecoder Latency = " << TO_SECOND(bank->mat.predecoderLatency) << ",";
		outputFile << string(indent, ' ') << "    |--- Subarray Latency   = " << TO_SECOND(bank->mat.subarray.writeLatency) << ",";

		outputFile << string(indent, ' ') << "       |--- Row Decoder Latency = " << TO_SECOND(bank->mat.subarray.rowDecoder.writeLatency) << ",";
		outputFile << string(indent, ' ') << "       |--- Charge Latency      = " << TO_SECOND(bank->mat.subarray.chargeLatency) << ",";
	}
	// Uncomment this if you are working with DRAM
    if (cell->memCellType == eDRAM || cell->memCellType == DRAM) {
        outputFile << string(indent, ' ') << " - Refresh Latency = " << TO_SECOND(bank->refreshLatency) << ",";
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            outputFile << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.writeLatency * (bank->stackedDieCount-1)) << ",";
        } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            outputFile << string(indent, ' ') << " |--- TSV Latency    = " << TO_SECOND(bank->tsvArray.writeLatency * (bank->stackedDieCount-1)) << ",";
        }
		if (inputParameter->routingMode == h_tree)
			outputFile << string(indent, ' ') << " |--- H-Tree Latency = " << TO_SECOND(bank->routingRefreshLatency) << ",";
		else
			outputFile << string(indent, ' ') << " |--- Non-H-Tree Latency = " << TO_SECOND(bank->routingRefreshLatency) << ",";
		outputFile << string(indent, ' ') << " |--- Mat Latency    = " << TO_SECOND(bank->mat.refreshLatency) << ",";
		outputFile << string(indent, ' ') << "    |--- Predecoder Latency = " << TO_SECOND(bank->mat.predecoderLatency) << ",";
		outputFile << string(indent, ' ') << "    |--- Subarray Latency   = " << TO_SECOND(bank->mat.subarray.refreshLatency) << ",";
    }

    if (cell->memCellType ==DWM) {
        outputFile << string(indent, ' ') << " - Shift Latency = " << TO_SECOND(bank->shiftLatency) << ",";
    }
	*/
	double numReadouts =  (double)bank->mat.subarray.numColumn / ((double)bank->blockSize / 
	(bank->numActiveMatPerColumn * bank->numActiveMatPerRow *  bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)) ;
	//cout << string(indent, ' ') << "- OUR number consecutive Readouts per subarray  = " << numReadouts << endl;
	//
	double avg_rowDecoder_latency = (double)bank->mat.subarray.rowDecoder.readLatency/numReadouts;
	//cout << string(indent, ' ') << "- OUR avg row decoder Latency   = " << avg_rowDecoder_latency << endl;

	// our correction:
	//Wordwidth / (subarray_latency - rowDecoder_latency + avg_rowDecoder_latency + Precharger_latency) / 8 (B/s)
	//avg version: 

	double readBandwidth = (double)bank->blockSize /
			(bank->mat.subarray.readLatency - bank->mat.subarray.rowDecoder.readLatency
			+ avg_rowDecoder_latency + bank->mat.subarray.precharger.readLatency) / 8;

	outputFile << string(indent, ' ') << TO_BPS(readBandwidth) << ","; //Read Bandwidth
        //outputFile<<" blocksize "<<bank->blockSize<<" rL "<<bank->mat.subarray.readLatency * 1e6<<" rL1 "<<bank->mat.subarray.rowDecoder.readLatency * 1e6<<" rL2 "<<bank->mat.subarray.precharger.readLatency * 1e6<<"\n";
	
	double readBandwidth_per_mat = (((double)bank->blockSize /(bank->numActiveMatPerColumn * bank->numActiveMatPerRow)) /
			(bank->mat.subarray.readLatency - bank->mat.subarray.rowDecoder.readLatency
			+ avg_rowDecoder_latency + bank->mat.subarray.precharger.readLatency) / 8);

	outputFile << string(indent, ' ') << TO_BPS(readBandwidth_per_mat) << ","; //Read Bandwidth per mat
	
	
	double writeBandwidth = (double)bank->blockSize /
			(bank->mat.subarray.writeLatency) / 8;
	outputFile << string(indent, ' ') << TO_BPS(writeBandwidth) << ","; //Write Bandwidth

	outputFile << string(indent, ' ') << TO_JOULE(bank->readDynamicEnergy) << ","; //Read Dynamic Energy (uJ)
	

    if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
        //cerr<<" using bank partitionGranularity as 0\n";
        // address and control bit dynamics + read data output dynamic
        outputFile << string(indent, ' ') << TO_JOULE(bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numReadBits + bank->tsvArray.readDynamicEnergy * bank->tsvArray.numDataBits * (bank->stackedDieCount-1)) << ",";//TSV Read Dynamic Energy
        //outputFile<<"SEE bank->tsvArray.writeDynamicEnergy "<<TO_JOULE(bank->tsvArray.writeDynamicEnergy)<<"\n";
	//outputFile<<"SEE bank->tsvArray.numReadBits "<<bank->tsvArray.numReadBits<<"\n";
	//outputFile<<"SEE bank->tsvArray.readDynamicEnergy "<<TO_JOULE(bank->tsvArray.readDynamicEnergy)<<"\n";
	//outputFile<<"SEE bank->tsvArray.numDataBits "<<bank->tsvArray.numDataBits<<"\n";
	
    } 
    else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
        // address bit dynamic + control bit dynamic + read data dynamic
        // TODO: revisit this
        //cerr<<" using bank partitionGranularity as 1\n";
        outputFile << string(indent, ' ') << TO_JOULE(bank->mat.tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * (bank->mat.tsvArray.numAccessBits) + bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->stackedDieCount + bank->tsvArray.readDynamicEnergy * bank->blockSize * (bank->stackedDieCount-1)) << ",";//TSV Read Dynamic Energy
    }
	else
		outputFile << string(indent, ' ') << "N/A" << ",";


	if (inputParameter->routingMode == h_tree){
		outputFile << string(indent, ' ') << TO_JOULE(bank->routingReadDynamicEnergy) << ","; //H-Tree Read Dynamic Energy (pJ)
		outputFile << string(indent, ' ') << "N/A" << ",";
	}
	else{
		outputFile << string(indent, ' ') << "N/A" << ",";
		outputFile << string(indent, ' ') << TO_JOULE(bank->routingReadDynamicEnergy) << ","; //Non-H-Tree Read Dynamic Energy (pJ)
	}
	outputFile << string(indent, ' ') << TO_JOULE(bank->mat.readDynamicEnergy) << ","; //Mat Read Dynamic Energy (pJ)
	outputFile << string(indent, ' ') << TO_JOULE(bank->mat.readDynamicEnergy - bank->mat.subarray.readDynamicEnergy
														* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
														<< ","; //Predecoder Read Dynamic Energy (pJ)
	outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.readDynamicEnergy) << ","; //Subarray Read Dynamic Energy (pJ)
	outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.rowDecoder.readDynamicEnergy) << ","; //Row Decoder Read Dynamic Energy (pJ)
	outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.bitlineMuxDecoder.readDynamicEnergy
													+ bank->mat.subarray.senseAmpMuxLev1Decoder.readDynamicEnergy
													+ bank->mat.subarray.senseAmpMuxLev2Decoder.readDynamicEnergy) << ","; //Mux Decoder Read Dynamic Energy (pJ)
	if (cell->memCellType == PCRAM || cell->memCellType == FBRAM || cell->memCellType == MRAM || cell->memCellType == memristor || cell->memCellType ==DWM) {
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.cellReadEnergy) << ","; //Cell Read Dynamic Energy (pJ)
	}
	else 
		outputFile << string(indent, ' ') << "N/A" << ",";

	if (inputParameter->internalSensing){
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.senseAmp.readDynamicEnergy) << ","; //Senseamp Read Dynamic Energy (pJ)
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.bitlineMux.readDynamicEnergy
													+ bank->mat.subarray.senseAmpMuxLev1.readDynamicEnergy
													+ bank->mat.subarray.senseAmpMuxLev2.readDynamicEnergy) << ","; //Mux Read Dynamic Energy (pJ)
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.precharger.readDynamicEnergy) << ","; //Precharge Read Dynamic Energy (pJ)
	}
	else {
		outputFile << string(indent, ' ') << "N/A" << ",";
		outputFile << string(indent, ' ') << "N/A" << ",";
		outputFile << string(indent, ' ') << "N/A" << ",";
	}
		
	if (cell->memCellType == PCRAM || cell->memCellType == FBRAM || cell->memCellType == DWM || cell->memCellType == MRAM ||
			(cell->memCellType == memristor && (cell->accessType == CMOS_access || cell->accessType == BJT_access))) {
		if(cell->memCellType==PCRAM && cell->memCellLevel!=SLC){
			outputFile << string(indent, ' ') << TO_JOULE(bank->resetDynamicEnergy+bank->setDynamicEnergy) << ","; //Set + Reset Dynamic Energy (pJ)
			//if (inputParameter->writeScheme==set_before_reset)
			//	outputFile << string(indent, ' ') << "  |--- Single Bit Write Energy = " <<TO_JOULE(cell->setEnergy+(cell->resetEnergy+cell->readPower*cell->readPulse)*cell->avgIterations)<<",";
			//else
			//	outputFile << string(indent, ' ') << "  |--- Single Bit Write Energy = " <<TO_JOULE(cell->resetEnergy+(cell->setEnergy+cell->readPower*cell->readPulse)*cell->avgIterations)<<",";

		}
		else 
			outputFile << string(indent, ' ') << "N/A" << ",";	


		outputFile << string(indent, ' ') << TO_JOULE(bank->writeDynamicEnergy ) << ","; //Write Dynamic Energy (pJ)

		outputFile << string(indent, ' ') << TO_JOULE(bank->resetDynamicEnergy) << ","; //Reset Dynamic Energy (pJ)
		if (cell->memCellType==MRAM && cell->memCellLevel!=SLC){
			outputFile << string(indent, ' ') << TO_JOULE(bank->resetDynamicEnergySoft) << ","; //Reset Soft Dynamic Energy (pJ)
		}
		else 
			outputFile << string(indent, ' ') << "N/A" << ",";
		

        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            outputFile << string(indent, ' ') << TO_JOULE(bank->tsvArray.resetDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numAccessBits) << ","; //TSV Reset Dynamic Energy
           //outputFile<<"SEE1 bank->tsvArray.resetDynamicEnergy "<<bank->tsvArray.resetDynamicEnergy<<"\n";
	   //outputFile<<"SEE1 bank->tsvArray.numAccessBits "<<bank->tsvArray.numAccessBits<<"\n";
       	}
       	else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            outputFile << string(indent, ' ') << TO_JOULE(bank->tsvArray.resetDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numAccessBits) << ","; //TSV Reset Dynamic Energy
        }
		else
			outputFile << string(indent, ' ') << "N/A" << ",";


		if (inputParameter->routingMode == h_tree){
			outputFile << string(indent, ' ') << TO_JOULE(bank->routingResetDynamicEnergy) << ","; //H-Tree Rest Dynamic Energy
			outputFile << string(indent, ' ') << "N/A" << ",";
		}
		else{
			outputFile << string(indent, ' ') << "N/A" << ",";
			outputFile << string(indent, ' ') << TO_JOULE(bank->routingResetDynamicEnergy) << ","; //BUS Reset Dynamic Energy
		}

		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.resetDynamicEnergy) << ","; //Mat Dynamic Energy
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.writeDynamicEnergy - bank->mat.subarray.writeDynamicEnergy 
															* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
															<< ","; //Predecoder Dynamic Energy
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.writeDynamicEnergy) << ","; //Subarray Dynamic Energy 
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.rowDecoder.writeDynamicEnergy) << ",";//Row Decoder Dynamic Energy
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.bitlineMuxDecoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1Decoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2Decoder.writeDynamicEnergy) << ",";//Mux Decoder Dynamic Energy
		outputFile << string(indent, ' ')  << TO_JOULE(bank->mat.subarray.bitlineMux.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2.writeDynamicEnergy) << ",";//Mux Dynamic Energy 
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.cellResetEnergy)<<":"<< TO_JOULE(bank->mat.subarray.cellResetEnergySoft)<< ",";
		outputFile << string(indent, ' ') << TO_JOULE(cell->resetEnergy)<<":"<< TO_JOULE(cell->resetEnergySoft)  << ","; //Single Cell RESET Energy 
	

		outputFile << string(indent, ' ') << TO_JOULE(bank->setDynamicEnergy) << ","; //Set Dynamic energy
		if (cell->memCellType==MRAM && cell->memCellLevel!=SLC){
			outputFile << string(indent, ' ') << TO_JOULE(bank->setDynamicEnergySoft) << ","; //Set Soft Dynamic energy
		}
		else 
			outputFile << string(indent, ' ') << "N/A" << ",";


		if (inputParameter->routingMode == h_tree){
			outputFile << string(indent, ' ') << TO_JOULE(bank->routingSetDynamicEnergy) << ","; //H-Tree Set Dynamic Energy
			outputFile << string(indent, ' ') << "N/A" << ",";
		}
		else{
			outputFile << string(indent, ' ') << "N/A" << ",";
			outputFile << string(indent, ' ') << TO_JOULE(bank->routingSetDynamicEnergy) << ","; //Bus Set Dynamic Energy
			}

		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.setDynamicEnergy) << ","; //Mat Set Dynamic Energy 
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.writeDynamicEnergy - bank->mat.subarray.writeDynamicEnergy
															* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
															<< ","; //Predecoder Set Dynamic Energy
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.writeDynamicEnergy) << ",";//Subarray Set Dynamic Energy
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.rowDecoder.writeDynamicEnergy) << ","; // Row Decoder Set Dynamic Energy
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.bitlineMuxDecoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1Decoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2Decoder.writeDynamicEnergy) << ","; //Mux Decoder Set Dynamic Energy
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.bitlineMux.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2.writeDynamicEnergy) << ","; //Mux Set Dynamic Energy
		outputFile << string(indent, ' ') << TO_JOULE(bank->mat.subarray.cellSetEnergy) <<":"<< TO_JOULE(bank->mat.subarray.cellSetEnergySoft)<<":"<<TO_JOULE(bank->mat.subarray.writeDynamicEnergy)<<",";
		outputFile << string(indent, ' ') << TO_JOULE(cell->setEnergy)<<":"<< TO_JOULE(cell->setEnergySoft) << ","; // Single Cell SET Energy
		
	/* uncomment to get NAND 
	} else if (cell->memCellType == SLCNAND||cell->memCellType==MLCNAND) {
		outputFile << string(indent, ' ') << " - Erase Dynamic Energy = " << TO_JOULE(bank->resetDynamicEnergy) << " per block" << ",";
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            outputFile << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.resetDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numAccessBits) << ",";
        //outputFile<<"SEE2 bank->tsvArray.resetDynamicEnergy "<<bank->tsvArray.resetDynamicEnergy<<"\n";
	//outputFile<<"SEE2 bank->tsvArray.numAccessBits "<<bank->tsvArray.numAccessBits<<"\n";
	} 
	else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            outputFile << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.resetDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numAccessBits) << ",";
        }
		if (inputParameter->routingMode == h_tree)
			outputFile << string(indent, ' ') << " |--- H-Tree Dynamic Energy = " << TO_JOULE(bank->routingResetDynamicEnergy) << ",";
		else
			outputFile << string(indent, ' ') << " |--- Non-H-Tree Dynamic Energy = " << TO_JOULE(bank->routingResetDynamicEnergy) << ",";
		outputFile << string(indent, ' ') << " |--- Mat Dynamic Energy    = " << TO_JOULE(bank->mat.resetDynamicEnergy) << " per mat" << ",";
		outputFile << string(indent, ' ') << "    |--- Predecoder Dynamic Energy = " << TO_JOULE(bank->mat.writeDynamicEnergy - bank->mat.subarray.writeDynamicEnergy
															* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
															<< ",";
		outputFile << string(indent, ' ') << "    |--- Subarray Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.writeDynamicEnergy) << " per active subarray" << ",";
		outputFile << string(indent, ' ') << "       |--- Row Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.rowDecoder.writeDynamicEnergy) << ",";
		outputFile << string(indent, ' ') << "       |--- Mux Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.bitlineMuxDecoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1Decoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2Decoder.writeDynamicEnergy) << ",";
		outputFile << string(indent, ' ') << "       |--- Mux Dynamic Energy         = " << TO_JOULE(bank->mat.subarray.bitlineMux.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2.writeDynamicEnergy) << ",";
		outputFile << string(indent, ' ') << " - Programming Dynamic Energy = " << TO_JOULE(bank->setDynamicEnergy) << " per page" << ",";
		if (inputParameter->routingMode == h_tree)
			outputFile << string(indent, ' ') << " |--- H-Tree Dynamic Energy = " << TO_JOULE(bank->routingSetDynamicEnergy) << ",";
		else
			outputFile << string(indent, ' ') << " |--- Non-H-Tree Dynamic Energy = " << TO_JOULE(bank->routingSetDynamicEnergy) << ",";
		outputFile << string(indent, ' ') << " |--- Mat Dynamic Energy    = " << TO_JOULE(bank->mat.setDynamicEnergy) << " per mat" << ",";
		outputFile << string(indent, ' ') << "    |--- Predecoder Dynamic Energy = " << TO_JOULE(bank->mat.writeDynamicEnergy - bank->mat.subarray.writeDynamicEnergy
															* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
															<< ",";
		outputFile << string(indent, ' ') << "    |--- Subarray Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.writeDynamicEnergy) << " per active subarray" << ",";
		outputFile << string(indent, ' ') << "       |--- Row Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.rowDecoder.writeDynamicEnergy) << ",";
		outputFile << string(indent, ' ') << "       |--- Mux Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.bitlineMuxDecoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1Decoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2Decoder.writeDynamicEnergy) << ",";
		outputFile << string(indent, ' ') << "       |--- Mux Dynamic Energy         = " << TO_JOULE(bank->mat.subarray.bitlineMux.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2.writeDynamicEnergy) << ",";
	*/
	//}  
	//IF NOT PCRAM but DRAM ;)
	//else {
	//	outputFile << string(indent, ' ') << " - Write Dynamic Energy = " << TO_JOULE(bank->writeDynamicEnergy) << ",";
    //    if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
    //        outputFile << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numAccessBits) << ",";
            //outputFile<<"SEE4 bank->tsvArray.resetDynamicEnergy "<<bank->tsvArray.resetDynamicEnergy<<"\n";
	    //outputFile<<"SEE4 bank->tsvArray.numAccessBits "<<bank->tsvArray.numAccessBits<<"\n";

	//}
	/*else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            outputFile << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numAccessBits) << ",";
        }
		if (inputParameter->routingMode == h_tree)
			outputFile << string(indent, ' ') << " |--- H-Tree Dynamic Energy = " << TO_JOULE(bank->routingWriteDynamicEnergy) << ",";
		else
			outputFile << string(indent, ' ') << " |--- Non-H-Tree Dynamic Energy = " << TO_JOULE(bank->routingWriteDynamicEnergy) << ",";
		outputFile << string(indent, ' ') << " |--- Mat Dynamic Energy    = " << TO_JOULE(bank->mat.writeDynamicEnergy) << " per mat" << ",";
		outputFile << string(indent, ' ') << "    |--- Predecoder Dynamic Energy = " << TO_JOULE(bank->mat.writeDynamicEnergy - bank->mat.subarray.writeDynamicEnergy
															* bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn)
															<< ",";
		outputFile << string(indent, ' ') << "    |--- Subarray Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.writeDynamicEnergy) << " per active subarray" << ",";
		outputFile << string(indent, ' ') << "       |--- Row Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.rowDecoder.writeDynamicEnergy) << ",";
		outputFile << string(indent, ' ') << "       |--- Mux Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.bitlineMuxDecoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1Decoder.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2Decoder.writeDynamicEnergy) << ",";
		outputFile << string(indent, ' ') << "       |--- Mux Dynamic Energy         = " << TO_JOULE(bank->mat.subarray.bitlineMux.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev1.writeDynamicEnergy
														+ bank->mat.subarray.senseAmpMuxLev2.writeDynamicEnergy) << ",";
		if (cell->memCellType == MRAM) {
			outputFile << string(indent, ' ') << "       |--- Bitline & Cell Write Energy= " << TO_JOULE(bank->mat.subarray.cellResetEnergy) << ",";
		}
	}
	//uncomment to get this and add columns DRAM!
    if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
        outputFile << string(indent, ' ') << " - Refresh Dynamic Energy = " << TO_JOULE(bank->refreshDynamicEnergy) << ",";
        if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
            outputFile << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numReadBits) << ",";
        } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
            outputFile << string(indent, ' ') << " |--- TSV Dynamic Energy    = " << TO_JOULE(bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numReadBits) << ",";
        }
		if (inputParameter->routingMode == h_tree)
			outputFile << string(indent, ' ') << " |--- H-Tree Dynamic Energy = " << TO_JOULE(bank->routingRefreshDynamicEnergy) << ",";
		else
			outputFile << string(indent, ' ') << " |--- Non-H-Tree Dynamic Energy = " << TO_JOULE(bank->routingRefreshDynamicEnergy) << ",";
        outputFile << string(indent, ' ') << " |--- Mat Dynamic Energy    = " << TO_JOULE(bank->mat.refreshDynamicEnergy) << " per mat" << ",";
        outputFile << string(indent, ' ') << "    |--- Predecoder Dynamic Energy = " << 
                TO_JOULE(bank->mat.refreshDynamicEnergy - bank->mat.subarray.refreshDynamicEnergy
                         * bank->numActiveSubarrayPerRow * bank->numActiveSubarrayPerColumn) << ",";
        outputFile << string(indent, ' ') << "    |--- Subarray Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.refreshDynamicEnergy) << " per active subarray" << ",";
        outputFile << string(indent, ' ') << "       |--- Row Decoder Dynamic Energy = " << TO_JOULE(bank->mat.subarray.rowDecoder.refreshDynamicEnergy) << ",";
        if (inputParameter->internalSensing)
            outputFile << string(indent, ' ') << "       |--- Senseamp Dynamic Energy    = " << TO_JOULE(bank->mat.subarray.senseAmp.refreshDynamicEnergy) << ",";
        outputFile << string(indent, ' ') << "       |--- Precharge Dynamic Energy   = " << TO_JOULE(bank->mat.subarray.precharger.refreshDynamicEnergy) << ",";
    }*/

	outputFile << string(indent, ' ') << TO_WATT(bank->leakage) << ","; //Leakage Power (W)
	
    if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) { // TSV leakage power
        outputFile << string(indent, ' ') << TO_WATT(bank->tsvArray.leakage * (bank->stackedDieCount-1) * bank->tsvArray.numTotalBits) << ",";
    } else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
        outputFile << string(indent, ' ') << TO_WATT(bank->tsvArray.leakage * (bank->stackedDieCount-1) * bank->tsvArray.numTotalBits + bank->mat.tsvArray.leakage * bank->numColumnMat * bank->numRowMat * bank->mat.tsvArray.numTotalBits) << ",";
    }
	else 
		outputFile << string(indent, ' ') << "N/A" << ",";

	if (inputParameter->routingMode == h_tree)
      {		outputFile << string(indent, ' ') << TO_WATT(bank->routingLeakage) << ","; //H-Tree Leakage Power (W)
	  		outputFile << string(indent, ' ') << "N/A" << ",";
          //outputFile<<"numRowMat "<<bank->numRowMat<<" numColumnMat "<<bank->numColumnMat<<"\n";
      }
	else{
		outputFile << string(indent, ' ') << TO_WATT(bank->routingLeakage) << ","; //Bus Leakage Power
		outputFile << string(indent, ' ') << "N/A" << ",";
	}
	outputFile << string(indent, ' ') << TO_WATT(bank->mat.leakage) << ","; //Mat Leakage Power (W)

    //if (cell->memCellType == eDRAM|| cell->memCellType == DRAM) {
    //    outputFile << string(indent, ' ') << " - Refresh Power = " << TO_WATT(bank->refreshDynamicEnergy / (cell->retentionTime)) << ",";
   	// }
    //if (cell->memCellType ==DWM) {
    //    outputFile << string(indent, ' ') << " - Shift Dynamic Energy = " << TO_JOULE(bank->shiftDynamicEnergy) << ",";
   	// }

	double dynamic_read_power = readBandwidth * (bank->readDynamicEnergy / ((double)bank->blockSize / 8) ); //Read Dynamic Power calculations
	outputFile << string(indent, ' ') << TO_WATT(dynamic_read_power) << ","; //Read Dynamic Power


    if (bank->stackedDieCount > 1 && bank->partitionGranularity == 0) {
        // address and control bit dynamics + read data output dynamic
		double tsv_dynamic_power = readBandwidth * ((bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->tsvArray.numReadBits + bank->tsvArray.readDynamicEnergy * bank->tsvArray.numDataBits * (bank->stackedDieCount-1))/ 8);
        outputFile << string(indent, ' ') << TO_WATT(tsv_dynamic_power) << ","; //TSV Dynamic Power

	
    } 
    else if (bank->stackedDieCount > 1 && bank->partitionGranularity == 1) {
        // address bit dynamic + control bit dynamic + read data dynamic
        // TODO: revisit this
		double tsv_dynamic_power = readBandwidth * ((bank->mat.tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * (bank->mat.tsvArray.numAccessBits) + bank->tsvArray.writeDynamicEnergy * (bank->stackedDieCount-1) * bank->stackedDieCount + bank->tsvArray.readDynamicEnergy * bank->blockSize * (bank->stackedDieCount-1))/8);
        outputFile << string(indent, ' ') << TO_WATT(tsv_dynamic_power) << ",";//TSV Read Dynamic Energy
    }
	else
		outputFile << string(indent, ' ') << "N/A" << ",";

	}

}
}


