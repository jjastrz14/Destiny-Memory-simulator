//Copyright (c) 2017-2019, IIT Hyderabad. See LICENSE file in the top-level directory
//Copyright (c) 2015-2016, UT-Battelle, LLC. See LICENSE file in the top-level directory
// This file contains code from NVSim, (c) 2012-2013,  Pennsylvania State University 
//and Hewlett-Packard Company. See LICENSE_NVSim file in the top-level directory.
//No part of DESTINY Project, including this file, may be copied,
//modified, propagated, or distributed except according to the terms
//contained in the LICENSE file.


#include "MemCell.h"
#include "formula.h"
#include "global.h"
#include "macros.h"
#include <math.h>

MemCell::MemCell() {
	// TODO Auto-generated constructor stub
	memCellType         = PCRAM;
	memCellLevel		= SLC;
	area                = 0;
	aspectRatio         = 0;
	resistanceOn        = 0;
	resistanceOff       = 0;
	readMode            = true;
	readVoltage         = 0;
	readCurrent         = 0;
	readPower           = 0;
	readPulse			= 0;
    wordlineBoostRatio  = 1.0;
	resetMode           = true;
	resetVoltage        = 0;
	resetCurrent        = 0;
	minSenseVoltage     = 0.08;
	resetPulse          = 0;
	resetEnergy         = 0;
	setMode             = true;
	setVoltage          = 0;
	setCurrent          = 0;
	setPulse            = 0;
	accessType          = CMOS_access;
	processNode         = 0;
	setEnergy           = 0;

	/* Optional */
	stitching         = 0;
	gateOxThicknessFactor = 2;
	widthSOIDevice = 0;
	widthAccessCMOS   = 0;
	voltageDropAccessDevice = 0;
	leakageCurrentAccessDevice = 0;
	capDRAMCell		  = 0;
	widthSRAMCellNMOS = 2.08;	/* Default NMOS width in SRAM cells is 2.08 (from CACTI) */
	widthSRAMCellPMOS = 1.23;	/* Default PMOS width in SRAM cells is 1.23 (from CACTI) */

	/*For memristors */
	readFloating = false;
	resistanceOnAtSetVoltage = 0;
	resistanceOffAtSetVoltage = 0;
	resistanceOnAtResetVoltage = 0;
	resistanceOffAtResetVoltage = 0;
	resistanceOnAtReadVoltage = 0;
	resistanceOffAtReadVoltage = 0;
	resistanceOnAtHalfReadVoltage = 0;
	resistanceOffAtHalfReadVoltage = 0;

	/* For eDRAM */
    retentionTime = invalid_value;

    /* For DWM */
    shiftCurrent = 0;
    shiftPulse = 0;
    shiftEnergy = 0;
    /* For MLC RRAM and PCRAM*/
    avgIterations = 0;

    /* For MLC MRAM*/
	resetVoltageSoft = 0;
	resetCurrentSoft = 0;
	resetPulseSoft = 0;
	resetEnergySoft = 0;
	setVoltageSoft = 0;
	setCurrentSoft = 0;
	setPulseSoft = 0;
	setEnergySoft = 0;


}

MemCell::~MemCell() {
	// TODO Auto-generated destructor stub
}

void MemCell::ReadCellFromFile(const string & inputFile)
{
	FILE *fp = fopen(inputFile.c_str(), "r");
	char line[5000];
	char tmp[5000];

	if (!fp) {
		cerr << inputFile << " cannot be found!\n";
		cerr<<" This file may be present in \"config\" folder. If so, please run destiny from that folder, otherwise, change the file name to include folder location.\n";
		exit(-1);
	}

	inputFileName = inputFile;

	while (fscanf(fp, "%[^\n]\n", line) != EOF) {
		if (!strncmp("-MemCellType", line, strlen("-MemCellType"))) {
			sscanf(line, "-MemCellType: %s", tmp);
			if (!strcmp(tmp, "SRAM"))
				memCellType = SRAM;
			else if (!strcmp(tmp, "DRAM"))
				memCellType = DRAM;
			else if (!strcmp(tmp, "eDRAM"))
				memCellType = eDRAM;
			else if (!strcmp(tmp, "MRAM"))
				memCellType = MRAM;
			else if (!strcmp(tmp, "PCRAM"))
				memCellType = PCRAM;
			else if (!strcmp(tmp, "FBRAM"))
				memCellType = FBRAM;
			else if (!strcmp(tmp, "memristor"))
				memCellType = memristor;
			else if (!strcmp(tmp, "SLCNAND"))
				memCellType = SLCNAND;
			else if (!strcmp(tmp, "MLCNAND"))
				memCellType = MLCNAND;
			else if (!strcmp(tmp, "DWM"))
				memCellType = DWM;
			else
				memCellType = SRAM;
			continue;
		}

		if (!strncmp("-MemCellLevel", line, strlen("-MemCellLevel"))) {
			sscanf(line, "-MemCellLevel: %s", tmp);
			if (!strcmp(tmp, "MLC"))
				memCellLevel = MLC;
			else if(!strcmp(tmp, "TLC"))
				memCellLevel = TLC;
			else
				memCellLevel = SLC;
			continue;
		}

		if (!strncmp("-ProcessNode", line, strlen("-ProcessNode"))) {
			sscanf(line, "-ProcessNode: %d", &processNode);
			continue;
		}
		if (!strncmp("-CellArea", line, strlen("-CellArea"))) {
			sscanf(line, "-CellArea (F^2): %lf", &area);
			continue;
		}
		if (!strncmp("-CellAspectRatio", line, strlen("-CellAspectRatio"))) {
			sscanf(line, "-CellAspectRatio: %lf", &aspectRatio);
			heightInFeatureSize = sqrt(area * aspectRatio);
			widthInFeatureSize = sqrt(area / aspectRatio);
			continue;
		}

		if (!strncmp("-ResistanceOnAtSetVoltage", line, strlen("-ResistanceOnAtSetVoltage"))) {
			sscanf(line, "-ResistanceOnAtSetVoltage (ohm): %lf", &resistanceOnAtSetVoltage);
			continue;
		}
		if (!strncmp("-ResistanceOffAtSetVoltage", line, strlen("-ResistanceOffAtSetVoltage"))) {
			sscanf(line, "-ResistanceOffAtSetVoltage (ohm): %lf", &resistanceOffAtSetVoltage);
			continue;
		}
		if (!strncmp("-ResistanceOnAtResetVoltage", line, strlen("-ResistanceOnAtResetVoltage"))) {
			sscanf(line, "-ResistanceOnAtResetVoltage (ohm): %lf", &resistanceOnAtResetVoltage);
			continue;
		}
		if (!strncmp("-ResistanceOffAtResetVoltage", line, strlen("-ResistanceOffAtResetVoltage"))) {
			sscanf(line, "-ResistanceOffAtResetVoltage (ohm): %lf", &resistanceOffAtResetVoltage);
			continue;
		}
		if (!strncmp("-ResistanceOnAtReadVoltage", line, strlen("-ResistanceOnAtReadVoltage"))) {
			sscanf(line, "-ResistanceOnAtReadVoltage (ohm): %lf", &resistanceOnAtReadVoltage);
			resistanceOn = resistanceOnAtReadVoltage;
			continue;
		}
		if (!strncmp("-ResistanceOffAtReadVoltage", line, strlen("-ResistanceOffAtReadVoltage"))) {
			sscanf(line, "-ResistanceOffAtReadVoltage (ohm): %lf", &resistanceOffAtReadVoltage);
			resistanceOff = resistanceOffAtReadVoltage;
			continue;
		}
		if (!strncmp("-ResistanceOnAtHalfReadVoltage", line, strlen("-ResistanceOnAtHalfReadVoltage"))) {
			sscanf(line, "-ResistanceOnAtHalfReadVoltage (ohm): %lf", &resistanceOnAtHalfReadVoltage);
			continue;
		}
		if (!strncmp("-ResistanceOffAtHalfReadVoltage", line, strlen("-ResistanceOffAtHalfReadVoltage"))) {
			sscanf(line, "-ResistanceOffAtHalfReadVoltage (ohm): %lf", &resistanceOffAtHalfReadVoltage);
			continue;
		}
		if (!strncmp("-ResistanceOnAtHalfResetVoltage", line, strlen("-ResistanceOnAtHalfResetVoltage"))) {
			sscanf(line, "-ResistanceOnAtHalfResetVoltage (ohm): %lf", &resistanceOnAtHalfResetVoltage);
			continue;
		}

		if (!strncmp("-ResistanceOn", line, strlen("-ResistanceOn"))) {
			sscanf(line, "-ResistanceOn (ohm): %lf", &resistanceOn);
			continue;
		}
		if (!strncmp("-ResistanceOff", line, strlen("-ResistanceOff"))) {
			sscanf(line, "-ResistanceOff (ohm): %lf", &resistanceOff);
			continue;
		}
		if (!strncmp("-CapacitanceOn", line, strlen("-CapacitanceOn"))) {
			sscanf(line, "-CapacitanceOn (F): %lf", &capacitanceOn);
			continue;
		}
		if (!strncmp("-CapacitanceOff", line, strlen("-CapacitanceOff"))) {
			sscanf(line, "-CapacitanceOff (F): %lf", &capacitanceOff);
			continue;
		}

		if (!strncmp("-GateOxThicknessFactor", line, strlen("-GateOxThicknessFactor"))) {
			sscanf(line, "-GateOxThicknessFactor: %lf", &gateOxThicknessFactor);
			continue;
		}

		if (!strncmp("-SOIDeviceWidth (F)", line, strlen("-SOIDeviceWidth (F)"))) {
			sscanf(line, "-SOIDeviceWidth (F): %lf", &widthSOIDevice);
			continue;
		}

		if (!strncmp("-ReadMode", line, strlen("-ReadMode"))) {
			sscanf(line, "-ReadMode: %s", tmp);
			if (!strcmp(tmp, "voltage"))
				readMode = true;
			else
				readMode = false;
			continue;
		}
		if (!strncmp("-ReadVoltage", line, strlen("-ReadVoltage"))) {
			sscanf(line, "-ReadVoltage (V): %lf", &readVoltage);
			continue;
		}
		if (!strncmp("-ReadCurrent", line, strlen("-ReadCurrent"))) {
			sscanf(line, "-ReadCurrent (uA): %lf", &readCurrent);
			readCurrent /= 1e6;
			continue;
		}

		if (!strncmp("-ReadPower", line, strlen("-ReadPower"))) {
			sscanf(line, "-ReadPower (uW): %lf", &readPower);
			readPower /= 1e6;
			continue;
		}
		if (!strncmp("-ReadPulse", line, strlen("-ReadPulse"))) {
			sscanf(line, "-ReadPulse (ns): %lf", &readPulse);
			readPulse /= 1e9;
			continue;
		}
		if (!strncmp("-WordlineBoostRatio", line, strlen("-WordlineBoostRatio"))) {
			sscanf(line, "-WordlineBoostRatio: %lf", &wordlineBoostRatio);
			continue;
		}
		if (!strncmp("-MinSenseVoltage", line, strlen("-MinSenseVoltage"))) {
			sscanf(line, "-MinSenseVoltage (mV): %lf", &minSenseVoltage);
			minSenseVoltage /= 1e3;
			continue;
		}


		if (!strncmp("-ResetMode", line, strlen("-ResetMode"))) {
			sscanf(line, "-ResetMode: %s", tmp);
			if (!strcmp(tmp, "voltage"))
				resetMode = true;
			else
				resetMode = false;
			continue;
		}
		if (!strncmp("-ResetVoltage", line, strlen("-ResetVoltage"))) {
            sscanf(line, "-ResetVoltage (V): %s", tmp);
            if (!strcmp(tmp, "vdd"))
                resetVoltage = tech->vdd;
            else
                sscanf(line, "-ResetVoltage (V): %lf", &resetVoltage);
			continue;
		}
		if (!strncmp("-ResetCurrent", line, strlen("-ResetCurrent"))) {
			sscanf(line, "-ResetCurrent (uA): %lf", &resetCurrent);
			resetCurrent /= 1e6;
			continue;
		}
		if (!strncmp("-ResetPulse", line, strlen("-ResetPulse"))) {
			sscanf(line, "-ResetPulse (ns): %lf", &resetPulse);
			resetPulse /= 1e9;
			continue;
		}
		if (!strncmp("-ResetEnergy", line, strlen("-ResetEnergy"))) {
			sscanf(line, "-ResetEnergy (pJ): %lf", &resetEnergy);
			resetEnergy /= 1e12;
			continue;
		}

		if (!strncmp("-SoftResetVoltage", line, strlen("-SoftResetVoltage"))) {
            sscanf(line, "-SoftResetVoltage (V): %s", tmp);
            if (!strcmp(tmp, "vdd"))
                resetVoltageSoft = tech->vdd;
            else
                sscanf(line, "-SoftResetVoltage(V): %lf", &resetVoltageSoft);
			continue;
		}
		if (!strncmp("-SoftResetCurrent", line, strlen("-SoftResetCurrent"))) {
			sscanf(line, "-SoftResetCurrent (uA): %lf", &resetCurrentSoft);
			resetCurrentSoft /= 1e6;
			continue;
		}
		if (!strncmp("-SoftResetPulse", line, strlen("-SoftResetPulse"))) {
			sscanf(line, "-SoftResetPulse (ns): %lf", &resetPulseSoft);
			resetPulseSoft /= 1e9;
			continue;
		}
		if (!strncmp("-SoftResetEnergy", line, strlen("-SoftResetEnergy"))) {
			sscanf(line, "-SoftResetEnergy (pJ): %lf", &resetEnergySoft);
			resetEnergySoft /= 1e12;
			continue;
		}

		if (!strncmp("-SetMode", line, strlen("-SetMode"))) {
			sscanf(line, "-SetMode: %s", tmp);
			if (!strcmp(tmp, "voltage"))
				setMode = true;
			else
				setMode = false;
			continue;
		}
		if (!strncmp("-SetVoltage", line, strlen("-SetVoltage"))) {
            sscanf(line, "-SetVoltage (V): %s", tmp);
            if (!strcmp(tmp, "vdd"))
                setVoltage = tech->vdd;
            else
                sscanf(line, "-SetVoltage (V): %lf", &setVoltage);
			continue;
		}
		if (!strncmp("-SetCurrent", line, strlen("-SetCurrent"))) {
			sscanf(line, "-SetCurrent (uA): %lf", &setCurrent);
			setCurrent /= 1e6;
			continue;
		}
		if (!strncmp("-SetPulse", line, strlen("-SetPulse"))) {
			sscanf(line, "-SetPulse (ns): %lf", &setPulse);
			setPulse /= 1e9;
			continue;
		}
		if (!strncmp("-SetEnergy", line, strlen("-SetEnergy"))) {
			sscanf(line, "-SetEnergy (pJ): %lf", &setEnergy);
			setEnergy /= 1e12;
			continue;
		}

		if (!strncmp("-SoftSetVoltage", line, strlen("-SoftSetVoltage"))) {
            sscanf(line, "-SoftSetVoltage (V): %s", tmp);
            if (!strcmp(tmp, "vdd"))
                setVoltage = tech->vdd;
            else
                sscanf(line, "-SoftSetVoltage (V): %lf", &setVoltageSoft);
			continue;
		}
		if (!strncmp("-SoftSetCurrent", line, strlen("-SoftSetCurrent"))) {
			sscanf(line, "-SoftSetCurrent (uA): %lf", &setCurrentSoft);
			setCurrentSoft /= 1e6;
			continue;
		}
		if (!strncmp("-SoftSetPulse", line, strlen("-SoftSetPulse"))) {
			sscanf(line, "-SoftSetPulse (ns): %lf", &setPulseSoft);
			setPulseSoft /= 1e9;
			continue;
		}
		if (!strncmp("-SoftSetEnergy", line, strlen("-SoftSetEnergy"))) {
			sscanf(line, "-SoftSetEnergy (pJ): %lf", &setEnergySoft);
			setEnergySoft /= 1e12;
			continue;
		}

		if (!strncmp("-AverageIterations", line, strlen("-AverageIterations"))) {
			if (memCellLevel == SLC)
				cout << "Warning: The write iteration is ignored because the memory cell is SLC." << endl;
			sscanf(line, "-AverageIterations: %lf", &avgIterations);
			continue;
		}
		if (!strncmp("-Interval", line, strlen("-Interval"))) {
			if (memCellLevel == SLC)
				cout << "Warning: The write iteration is ignored because the memory cell is SLC." << endl;
			sscanf(line, "-Interval (ns): %lf", &interval);
			interval/=1e9;
			continue;
		}

		if (!strncmp("-Stitching", line, strlen("-Stitching"))) {
			sscanf(line, "-Stitching: %d", &stitching);
			continue;
		}

		if (!strncmp("-AccessType", line, strlen("-AccessType"))) {
			sscanf(line, "-AccessType: %s", tmp);
			if (!strcmp(tmp, "CMOS"))
				accessType = CMOS_access;
			else if (!strcmp(tmp, "BJT"))
				accessType = BJT_access;
			else if (!strcmp(tmp, "diode"))
				accessType = diode_access;
			else
				accessType = none_access;
			continue;
		}

		if (!strncmp("-AccessCMOSWidth", line, strlen("-AccessCMOSWidth"))) {
			if (accessType != CMOS_access)
				cout << "Warning: The input of CMOS access transistor width is ignored because the cell is not CMOS-accessed." << endl;
			else
				sscanf(line, "-AccessCMOSWidth (F): %lf", &widthAccessCMOS);
			continue;
		}

		if (!strncmp("-VoltageDropAccessDevice", line, strlen("-VoltageDropAccessDevice"))) {
			sscanf(line, "-VoltageDropAccessDevice (V): %lf", &voltageDropAccessDevice);
			continue;
		}

		if (!strncmp("-LeakageCurrentAccessDevice", line, strlen("-LeakageCurrentAccessDevice"))) {
			sscanf(line, "-LeakageCurrentAccessDevice (uA): %lf", &leakageCurrentAccessDevice);
			leakageCurrentAccessDevice /= 1e6;
			continue;
		}

		if (!strncmp("-DRAMCellCapacitance", line, strlen("-DRAMCellCapacitance"))) {
			if (memCellType != DRAM && memCellType != eDRAM)
				cout << "Warning: The input of DRAM cell capacitance is ignored because the memory cell is not DRAM." << endl;
			else
				sscanf(line, "-DRAMCellCapacitance (F): %lf", &capDRAMCell);
			continue;
		}

		if (!strncmp("-SRAMCellNMOSWidth", line, strlen("-SRAMCellNMOSWidth"))) {
			if (memCellType != SRAM)
				cout << "Warning: The input of SRAM cell NMOS width is ignored because the memory cell is not SRAM." << endl;
			else
				sscanf(line, "-SRAMCellNMOSWidth (F): %lf", &widthSRAMCellNMOS);
			continue;
		}

		if (!strncmp("-SRAMCellPMOSWidth", line, strlen("-SRAMCellPMOSWidth"))) {
			if (memCellType != SRAM)
				cout << "Warning: The input of SRAM cell PMOS width is ignored because the memory cell is not SRAM." << endl;
			else
				sscanf(line, "-SRAMCellPMOSWidth (F): %lf", &widthSRAMCellPMOS);
			continue;
		}


		if (!strncmp("-ReadFloating", line, strlen("-ReadFloating"))) {
			sscanf(line, "-ReadFloating: %s", tmp);
			if (!strcmp(tmp, "true"))
				readFloating = true;
			else
				readFloating = false;
			continue;
		}

		if (!strncmp("-FlashEraseVoltage (V)", line, strlen("-FlashEraseVoltage (V)"))) {
			if (memCellType != SLCNAND && memCellType != MLCNAND)
				cout << "Warning: The input of programming/erase voltage is ignored because the memory cell is not flash." << endl;
			else
				sscanf(line, "-FlashEraseVoltage (V): %lf", &flashEraseVoltage);
			continue;
		}

		if (!strncmp("-FlashProgramVoltage (V)", line, strlen("-FlashProgramVoltage (V)"))) {
			if (memCellType != SLCNAND && memCellType != MLCNAND)
				cout << "Warning: The input of programming/program voltage is ignored because the memory cell is not flash." << endl;
			else
				sscanf(line, "-FlashProgramVoltage (V): %lf", &flashProgramVoltage);
			continue;
		}

		if (!strncmp("-FlashPassVoltage (V)", line, strlen("-FlashPassVoltage (V)"))) {
			if (memCellType != SLCNAND && memCellType != MLCNAND)
				cout << "Warning: The input of pass voltage is ignored because the memory cell is not flash." << endl;
			else
				sscanf(line, "-FlashPassVoltage (V): %lf", &flashPassVoltage);
			continue;
		}

		if (!strncmp("-FlashEraseTime", line, strlen("-FlashEraseTime"))) {
			if (memCellType != SLCNAND && memCellType != MLCNAND)
				cout << "Warning: The input of erase time is ignored because the memory cell is not flash." << endl;
			else {
				sscanf(line, "-FlashEraseTime (ms): %lf", &flashEraseTime);
				flashEraseTime /= 1e3;
			}
			continue;
		}

		if (!strncmp("-FlashProgramTime", line, strlen("-FlashProgramTime"))) {
			if (memCellType != SLCNAND && memCellType != MLCNAND)
				cout << "Warning: The input of erase time is ignored because the memory cell is not flash." << endl;
			else {
				sscanf(line, "-FlashProgramTime (us): %lf", &flashProgramTime);
				flashProgramTime /= 1e6;
			}
			continue;
		}

		if (!strncmp("-GateCouplingRatio", line, strlen("-GateCouplingRatio"))) {
			if (memCellType != SLCNAND && memCellType != MLCNAND)
				cout << "Warning: The input of gate coupling ratio (GCR) is ignored because the memory cell is not flash." << endl;
			else {
				sscanf(line, "-GateCouplingRatio: %lf", &gateCouplingRatio);
			}
			continue;
		}

		if (!strncmp("-RetentionTime", line, strlen("-RetentionTime"))) {
			if (memCellType != eDRAM)
				cout << "Warning: The input of retention time is ignored because the cell is not eDRAM." << endl;
			else {
				sscanf(line, "-RetentionTime (us): %lf", &retentionTime);
                retentionTime /= 1e6;
            }
			continue;
		}

		if (!strncmp("-Temperature", line, strlen("-Temperature"))) {
			if (memCellType != eDRAM)
				cout << "Warning: The input of temperature is ignored because the cell is not eDRAM." << endl;
			else
				sscanf(line, "-Temperature (K): %lf", &temperature);
			continue;
		}

		if (!strncmp("-TapeLength", line, strlen("-TapeLength"))) {
			if (memCellType != DWM)
				cout << "Warning: The domain length is ignored because the cell is not DWM." << endl;
			else{
				sscanf(line, "-TapeLength (bit): %lf", &tapeLength);
			}
			continue;
		}

		if (!strncmp("-PortDistance", line, strlen("-PortDistance"))) {
			if (memCellType != DWM)
				cout << "Warning: The port distance is ignored because the cell is not DWM." << endl;
			else{
				sscanf(line, "-PortDistance (bit): %lf", &portDistance);
			}
			continue;
		}
		if (!strncmp("-TapePerGroup", line, strlen("-TapePerGroup"))) {
			if (memCellType != DWM)
				cout << "Warning: The tape per group is ignored because the cell is not DWM." << endl;
			else{
				sscanf(line, "-TapePerGroup: %lf", &tapePerGroup);
			}
			continue;
		}

		if (!strncmp("-ShiftCurrent", line, strlen("-ShiftCurrent"))) {
			if (memCellType != DWM)
				cout << "Warning: The input of shift current is ignored because the cell is not DWM." << endl;
			else{
				sscanf(line, "-ShiftCurrent (uA): %lf", &shiftCurrent);
				shiftCurrent /=1e6;
			}
			continue;
		}

		if (!strncmp("-ShiftPulse", line, strlen("-ShiftPulse"))) {
			if (memCellType != DWM)
				cout << "Warning: The input of shift pulse is ignored because the cell is not DWM." << endl;
			else{
				sscanf(line, "-ShiftPulse (ns): %lf", &shiftPulse);
				shiftPulse /=1e9;
			}
			continue;
		}

		if (!strncmp("-ShiftEnergy", line, strlen("-ShiftEnergy"))) {
			if (memCellType != DWM)
				cout << "Warning: The input of shift energy is ignored because the cell is not DWM." << endl;
			else{
				sscanf(line, "-ShiftEnergy (pJ): %lf", &shiftEnergy);
				shiftEnergy /=1e12;
			}
			continue;
		}		
	}

	fclose(fp);
}


void MemCell::ApplyPVT() {
    if (retentionTime == invalid_value) {
        /* TODO: No given retention time, we should calculate it. */
        return;
    }

    if (memCellType == eDRAM) {
        cout << "[Info] Retention time given at " << temperature << "K is " << retentionTime * 1e6 << "us" << endl;
        double exponent = -0.0268 * (inputParameter->temperature - temperature);
        retentionTime = retentionTime * exp(exponent);
        cout << "[Info] Retention time at " << inputParameter->temperature << "K is " << retentionTime * 1e6 << "us" << endl;
    }
}


void MemCell::CellScaling(int _targetProcessNode) {
	if ((processNode > 0) && (processNode != _targetProcessNode)) {
		double scalingFactor = (double)processNode / _targetProcessNode;
		if (memCellType == PCRAM) {
			resistanceOn *= scalingFactor;
			resistanceOff *= scalingFactor;
			if (!setMode) {
				setCurrent /= scalingFactor;
			} else {
				setVoltage *= 1;
			}
			if (!resetMode) {
				resetCurrent /= scalingFactor;
			} else {
				resetVoltage *= 1;
			}
			if (accessType == diode_access) {
				capacitanceOn /= scalingFactor; //TO-DO
				capacitanceOff /= scalingFactor; //TO-DO
			}
		} else if (memCellType == MRAM){ //TO-DO: MRAM
			resistanceOn *= scalingFactor * scalingFactor;
			resistanceOff *= scalingFactor * scalingFactor;
			if (!setMode) {
				setCurrent /= scalingFactor;
			} else {
				setVoltage *= scalingFactor;
			}
			if (!resetMode) {
				resetCurrent /= scalingFactor;
			} else {
				resetVoltage *= scalingFactor;
			}
			if (accessType == diode_access) {
				capacitanceOn /= scalingFactor; //TO-DO
				capacitanceOff /= scalingFactor; //TO-DO
			}
		} else if (memCellType == memristor) { //TO-DO: memristor

		} else { //TO-DO: other RAMs

		}
		processNode = _targetProcessNode;
	}
}

double MemCell::GetMemristance(double _relativeReadVoltage) { /* Get the LRS resistance of memristor at log-linera region of I-V curve */
	if (memCellType == memristor) {
		double x1, x2, x3;  // x1: read voltage, x2: half voltage, x3: applied voltage
		if (readVoltage == 0) {
			x1 = readCurrent * resistanceOnAtReadVoltage;
		} else {
			x1 = readVoltage;
		}
		x2 = readVoltage / 2;
		x3 = _relativeReadVoltage * readVoltage;
		double y1, y2 ,y3; // y1:log(read current), y2: log(leakage current at half read voltage
		y1 = log2(x1/resistanceOnAtReadVoltage);
		y2 = log2(x2/resistanceOnAtHalfReadVoltage);
		y3 = (y2 - y1) / (x2 -x1) * x3 + (x2 * y1 - x1 * y2) / (x2 - x1);  //insertion
		return x3 / pow(2, y3);
	} else {  // not memristor, can't call the function
		cout <<"Warning[MemCell] : Try to get memristance from a non-memristor memory cell" << endl;
		return -1;
	}
}

void MemCell::CalculateShiftEnergy() {
	/*Model Based on GuangyuSun ASPDAC paper*/
	double R = 4.8*1e-7 *tapeLength*2*tech->featureSize/(6*1e-9 * 1*tech->featureSize);
	shiftEnergy = shiftCurrent*shiftCurrent*R*shiftPulse;
}

void MemCell::CalculateWriteEnergy() {
	if (resetEnergy == 0) {
		if (resetMode) {	//Voltage based reset
			if (memCellType == memristor)
				if (accessType == none_access)
					resetEnergy = fabs(resetVoltage) * (fabs(resetVoltage) - voltageDropAccessDevice) / resistanceOnAtResetVoltage * resetPulse;
				else
					resetEnergy = fabs(resetVoltage) * (fabs(resetVoltage) - voltageDropAccessDevice) / resistanceOn * resetPulse;
			else if (memCellType == PCRAM)
				resetEnergy = fabs(resetVoltage) * (fabs(resetVoltage) - voltageDropAccessDevice) / resistanceOn * resetPulse;	// PCM cells shows low resistance during most time of the switching
			else if (memCellType == FBRAM)
				resetEnergy = fabs(resetVoltage) * fabs(resetCurrent) * resetPulse;
			else
				resetEnergy = fabs(resetVoltage) * (fabs(resetVoltage) - voltageDropAccessDevice) / resistanceOn * resetPulse;
		} else {	//Current based reset
			if (resetVoltage == 0){
				resetEnergy = tech->vdd * fabs(resetCurrent) * resetPulse; /*TO-DO consider charge pump*/
			} else {
				resetEnergy = fabs(resetVoltage) * fabs(resetCurrent) * resetPulse;
			}
			/* previous model seems to be problematic
			if (memCellType == memristor)
				if (accessType == none_access)
					resetEnergy = resetCurrent * (resetCurrent * resistanceOffAtResetVoltage + voltageDropAccessDevice) * resetPulse;
				else
					resetEnergy = resetCurrent * (resetCurrent * resistanceOff + voltageDropAccessDevice) * resetPulse;
			else if (memCellType == PCRAM)
				resetEnergy = resetCurrent * (resetCurrent * resistanceOn + voltageDropAccessDevice) * resetPulse;		// PCM cells shows low resistance during most time of the switching
			else if (memCellType == FBRAM)
				resetEnergy = fabs(resetVoltage) * fabs(resetCurrent) * resetPulse;
			else
				resetEnergy = resetCurrent * (resetCurrent * resistanceOff + voltageDropAccessDevice) * resetPulse;
		    */
		}
	}

	if (setEnergy == 0) {
		if (setMode) {	//Voltage based set
			if (memCellType == memristor)
				if (accessType == none_access)
					setEnergy = fabs(setVoltage) * (fabs(setVoltage) - voltageDropAccessDevice) / resistanceOnAtSetVoltage * setPulse;
				else
					setEnergy = fabs(setVoltage) * (fabs(setVoltage) - voltageDropAccessDevice) / resistanceOn * setPulse;
			else if (memCellType == PCRAM)
				setEnergy = fabs(setVoltage) * (fabs(setVoltage) - voltageDropAccessDevice) / resistanceOn * setPulse;			// PCM cells shows low resistance during most time of the switching
			else if (memCellType == FBRAM)
				setEnergy = fabs(setVoltage) * fabs(setCurrent) * setPulse;
			else
				setEnergy = fabs(setVoltage) * (fabs(setVoltage) - voltageDropAccessDevice) / resistanceOn * setPulse;
		} else {	//Current based set
			if (setVoltage == 0){
				setEnergy = tech->vdd * fabs(setCurrent) * setPulse; /*TO-DO consider charge pump*/
			} else {
				setEnergy = fabs(setVoltage) * fabs(setCurrent) * setPulse;
			}
			/* previous model seems to be problematic
			if (memCellType == memristor)
				if (accessType == none_access)
					setEnergy = setCurrent * (setCurrent * resistanceOffAtSetVoltage + voltageDropAccessDevice) * setPulse;
				else
					setEnergy = setCurrent * (setCurrent * resistanceOff + voltageDropAccessDevice) * setPulse;
			else if (memCellType == PCRAM)
				setEnergy = setCurrent * (setCurrent * resistanceOn + voltageDropAccessDevice) * setPulse;		// PCM cells shows low resistance during most time of the switching
			else if (memCellType == FBRAM)
				setEnergy = fabs(setVoltage) * fabs(setCurrent) * setPulse;
			else
				setEnergy = setCurrent * (setCurrent * resistanceOff + voltageDropAccessDevice) * setPulse;
			*/
		}
	}
	/* For MLC MRAM*/
	if (resetEnergySoft == 0) {
		if (resetMode) {	//Voltage based reset
			resetEnergySoft = fabs(resetVoltageSoft) * (fabs(resetVoltageSoft) - voltageDropAccessDevice) / resistanceOn * resetPulseSoft;
		} else {	//Current based reset
			if (resetVoltageSoft == 0){
				resetEnergySoft = tech->vdd * fabs(resetCurrentSoft) * resetPulseSoft; /*TO-DO consider charge pump*/
			} else {
				resetEnergySoft = fabs(resetVoltageSoft) * fabs(resetCurrentSoft) * resetPulseSoft;
			}
		}
	}
	if (setEnergySoft == 0) {
		if (setMode) {	//Voltage based reset
			setEnergySoft = fabs(setVoltageSoft) * (fabs(setVoltageSoft) - voltageDropAccessDevice) / resistanceOn * setPulseSoft;
		} else {	//Current based reset
			if (setVoltageSoft == 0){
				setEnergySoft = tech->vdd * fabs(setCurrentSoft) * setPulseSoft; /*TO-DO consider charge pump*/
			} else {
				setEnergySoft = fabs(setVoltageSoft) * fabs(setCurrentSoft) * setPulseSoft;
			}
		}
	}
}

double MemCell::CalculateReadPower() { /* TO-DO consider charge pumped read voltage */
	if (readPower == 0) {
		if (cell->readMode) {	/* voltage-sensing */
			if (readVoltage == 0) { /* Current-in voltage sensing */
				return tech->vdd * readCurrent;
			}
			if (readCurrent == 0) { /*Voltage-divider sensing */
				double resInSerialForSenseAmp, maxBitlineCurrent;
				resInSerialForSenseAmp = sqrt(resistanceOn * resistanceOff);
				maxBitlineCurrent = (readVoltage - voltageDropAccessDevice) / (resistanceOn + resInSerialForSenseAmp);
				return tech->vdd * maxBitlineCurrent;
			}
		} else { /* current-sensing */
			double maxBitlineCurrent = (readVoltage - voltageDropAccessDevice) / resistanceOn;
			return tech->vdd * maxBitlineCurrent;
		}
	} else {
		return -1.0; /* should not call the function if read energy exists */
	}
	return -1.0;
}

void MemCell::PrintCell(int indent)
{
	switch (memCellType) {
	case SRAM:
		cout << string(indent, ' ') << "Memory Cell: SRAM" << endl;
		break;
	case DRAM:
		cout << string(indent, ' ') << "Memory Cell: DRAM" << endl;
		break;
	case eDRAM:
		cout << string(indent, ' ') << "Memory Cell: Embedded DRAM" << endl;
		break;
	case MRAM:
		cout << string(indent, ' ') << "Memory Cell: MRAM (Magnetoresistive)" << endl;
		break;
	case PCRAM:
		cout << string(indent, ' ') << "Memory Cell: PCRAM (Phase-Change)" << endl;
		break;
	case memristor:
		cout << string(indent, ' ') << "Memory Cell: RRAM (Memristor)" << endl;
		break;
	case FBRAM:
		cout << string(indent, ' ') << "Memory Cell: FBRAM (Floating Body)" <<endl;
		break;
	case SLCNAND:
		cout << string(indent, ' ') << "Memory Cell: Single-Level Cell NAND Flash" << endl;
		break;
	case MLCNAND:
		cout << string(indent, ' ') << "Memory Cell: Multi-Level Cell NAND Flash" << endl;
		break;
	case DWM:
		cout << string(indent, ' ') << "Memory Cell: DWM (Domain Wall Memory)" <<endl;
		break;
	default:
		cout << string(indent, ' ') << "Memory Cell: Unknown" << endl;
	}
	cout << string(indent, ' ') << "Cell Area (F^2)    : " << area << " (" << heightInFeatureSize << "Fx" << widthInFeatureSize << "F)" << endl;
	cout << string(indent, ' ') << "Cell Aspect Ratio  : " << aspectRatio << endl;

	if (memCellType == PCRAM || memCellType == MRAM || memCellType == memristor || memCellType == FBRAM || memCellType == DWM) {
		if (resistanceOn < 1e3 )
			cout << string(indent, ' ') << "Cell Turned-On Resistance : " << resistanceOn << "ohm" << endl;
		else if (resistanceOn < 1e6)
			cout << string(indent, ' ') << "Cell Turned-On Resistance : " << resistanceOn / 1e3 << "Kohm" << endl;
		else
			cout << string(indent, ' ') << "Cell Turned-On Resistance : " << resistanceOn / 1e6 << "Mohm" << endl;
		if (resistanceOff < 1e3 )
			cout << string(indent, ' ') << "Cell Turned-Off Resistance: "<< resistanceOff << "ohm" << endl;
		else if (resistanceOff < 1e6)
			cout << string(indent, ' ') << "Cell Turned-Off Resistance: "<< resistanceOff / 1e3 << "Kohm" << endl;
		else
			cout << string(indent, ' ') << "Cell Turned-Off Resistance: "<< resistanceOff / 1e6 << "Mohm" << endl;

		if (readMode) {
			cout << string(indent, ' ') << "Read Mode: Voltage-Sensing" << endl;
			if (readCurrent > 0)
				cout << string(indent, ' ') << "  - Read Current: " << readCurrent * 1e6 << "uA" << endl;
			if (readVoltage > 0)
				cout << string(indent, ' ') << "  - Read Voltage: " << readVoltage << "V" << endl;
		} else {
			cout << string(indent, ' ') << "Read Mode: Current-Sensing" << endl;
			if (readCurrent > 0)
				cout << string(indent, ' ') << "  - Read Current: " << readCurrent * 1e6 << "uA" << endl;
			if (readVoltage > 0)
				cout << string(indent, ' ') << "  - Read Voltage: " << readVoltage << "V" << endl;
		}

		if (resetMode) {
			cout << string(indent, ' ') << "Reset Mode: Voltage" << endl;
			cout << string(indent, ' ') << "  - Reset Voltage: " << resetVoltage << "V" << endl;
		} else {
			cout << string(indent, ' ') << "Reset Mode: Current" << endl;
			cout << string(indent, ' ') << "  - Reset Current: " << resetCurrent * 1e6 << "uA" << endl;
		}
		cout << string(indent, ' ') << "  - Reset Pulse: " << TO_SECOND(resetPulse) << endl;

		if (setMode) {
			cout << string(indent, ' ') << "Set Mode: Voltage" << endl;
			cout << string(indent, ' ') << "  - Set Voltage: " << setVoltage << "V" << endl;
		} else {
			cout << string(indent, ' ') << "Set Mode: Current" << endl;
			cout << string(indent, ' ') << "  - Set Current: " << setCurrent * 1e6 << "uA" << endl;
		}
		cout << string(indent, ' ') << "  - Set Pulse: " << TO_SECOND(setPulse) << endl;

		if (memCellType == DWM){
			cout << string(indent, ' ') << "  - Shift Current: " << shiftCurrent *1e6 <<"uA" << endl;
		}
		switch (accessType) {
		case CMOS_access:
			cout << string(indent, ' ') << "Access Type: CMOS" << endl;
			break;
		case BJT_access:
			cout << string(indent, ' ') << "Access Type: BJT" << endl;
			break;
		case diode_access:
			cout << string(indent, ' ') << "Access Type: Diode" << endl;
			break;
		default:
			cout << string(indent, ' ') << "Access Type: None Access Device" << endl;
		}
	} else if (memCellType == SRAM) {
		cout << string(indent, ' ') << "SRAM Cell Access Transistor Width: " << widthAccessCMOS << "F" << endl;
		cout << string(indent, ' ') << "SRAM Cell NMOS Width: " << widthSRAMCellNMOS << "F" << endl;
		cout << string(indent, ' ') << "SRAM Cell PMOS Width: " << widthSRAMCellPMOS << "F" << endl;
	} else if (memCellType == SLCNAND || memCellType == MLCNAND) {
		cout << string(indent, ' ') << "Pass Voltage       : " << flashPassVoltage << "V" << endl;
		cout << string(indent, ' ') << "Programming Voltage: " << flashProgramVoltage << "V" << endl;
		cout << string(indent, ' ') << "Erase Voltage      : " << flashEraseVoltage << "V" << endl;
		cout << string(indent, ' ') << "Programming Time   : " << TO_SECOND(flashProgramTime) << endl;
		cout << string(indent, ' ') << "Erase Time         : " << TO_SECOND(flashEraseTime) << endl;
		cout << string(indent, ' ') << "Gate Coupling Ratio: " << gateCouplingRatio << endl;
	}
}
