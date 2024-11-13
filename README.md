![DESTINY Logo](docs/destiny_framework.png)

# DESTINY: 3D Design-Space Exploration Tool

DESTINY (3D dEsign-Space exploraTIon Tool for SRAM, eDRAM and Non-volatile memorY) is a comprehensive modeling tool for both conventional and emerging memory technologies. It extends the capabilities of tools like CACTI, CACTI-3DD, and NVSim to provide extensive 3D memory modeling capabilities.

## Features

DESTINY can model:
- **2D/3D Memory Technologies:**
  - SRAM and eDRAM
  - STT-RAM, ReRAM, and PCM (2D/3D, SLC/MLC)
  - SOT-RAM, Flash, DWM (2D, SLC/MLC)

- **Key Capabilities:**
  - Support for 22nm to 180nm technology nodes
  - Comprehensive modeling of conventional and emerging memories
  - Validated against commercial prototypes
  - Fine and coarse-grained TSV modeling
  - Multi-level cell support

For more details refer to [DESTINY publication](docs/literature/2015_DESTINY%20-%20A%20Comprehensive%20Tool%20with%203D%20and%20Multi-Level%20Cell%20Memory%20Modeling%20Capability.pdf)

## Installation

DESTINY is developed in C++ and can be compiled on both Microsoft Windows and Unix-like operating systems.

### Building on Linux
```bash
$ make
```

## Usage

Run DESTINY with example configuration file:
```bash
$ ./destiny
```

For user-defined configuration files in a different directory:
```bash
$ ./destiny <path-to-file/file>.cfg
```

For configuration files in config folder you can also: 
```bash 
$ cd config 
$ ../destiny <file.cfg>
``` 

Assuming that <file>.cfg is present in the topfolder/config folder, whereas "destiny" binary is present in "topfolder".
Remember: .cfg file can refer to the another .cell file, so the exact path for .cell file needs to be provided in .cfg file. 

## Running First Simulation



see: [CACTI](docs/literature/2008_CACTI_5.1_technical_report_HPL.pdf)


## Configuration Parameters

To understand possible parameters which can be defined in .cfg and .cell files please refer to: 
- [Configuration parameters](docs/cfg_parameters.md)
- [Cell parameters](docs/cell_parameters.md)

Where more or less all possible input parameters where collected. 

## Support

Authors suggest for questions and support:
- Mailing List: destiny-help@elist.ornl.gov
- Archive: [Previous Discussions](https://elist.ornl.gov/pipermail/destiny-help/)
- This repository mailing list: jakubandrzej.jastrzebski@polimi.it

## Citing This Work

If you use DESTINY in a research publication, authors request you to cite any of the these publications. 

```bibtex
@article{mittal2017destiny,
  title={DESTINY: A Comprehensive Tool with {3D} and Multi-level Cell Memory Modeling Capability},
  author={Mittal, Sparsh and Wang, Rujia and Vetter, Jeffrey},
  journal={Journal of Low Power Electronics and Applications},
  year={2017},
  publisher={MDPI}
}

@techreport{mittal2014exploring,
  title={Exploring Design Space of {3D} {NVM} and {eDRAM} Caches Using {DESTINY} Tool},
  author={Mittal, Sparsh and Poremba, Matthew and Vetter, Jeffrey S and Xie, Yuan},
  year={2014},
  institution={Oak Ridge National Laboratory},
  number={ORNL/TM-2014/636},
  url={http://goo.gl/qzyWFE}
}

@inproceedings{poremba2015destiny,
  title={{DESTINY}: A Tool for Modeling Emerging {3D} {NVM} and {eDRAM} caches},
  author={Poremba, Matthew and Mittal, Sparsh and Li, Dong and Vetter, Jeffrey S and Xie, Yuan},
  booktitle={Design, Automation \& Test in Europe Conference \& Exhibition (DATE)},
  year={2015},
  url={http://goo.gl/3nKAM2}
}
```

The JLPEA 2017 paper describes DESTINY in full detail and also shows its use in performing design-space exploration.

Support for DESTINY is provided on a best-effort basis. For receiving announcements,
 or sending questions and comments, please subscribe to the mailing list
 destiny-help@elist.ornl.gov by visiting the following 
webpage: https://elist.ornl.gov/mailman/listinfo/destiny-help 

check: [Get in touch](##Get-in-touch)


## Sponsors

- Science and Engineering Research Board (SERB), India (ECR/2017/000622)
- Office of Advanced Scientific Computing Research, U.S. Department of Energy


## License and Disclaimer

This is not an officially supported Politecnico di Milano product.

Copyright 2024 Politecnico di Milano.

### Source Code License
This software is licensed under BSD-style licenses from multiple source projects. See the [LICENSE](LICENSE) file for detailed terms and conditions.

[![License](https://img.shields.io/badge/License-BSD--style-blue.svg)](LICENSE)

## Acknowledgments

DESTINY utilizes frameworks from:
- NVSim (2D SRAM and 2D NVM modeling) see: [NVsim](docs/literature/2012-NVsim-TCAD.pdf)
- CACTI-3DD (TSV modeling) see: [CACTI-3DD](docs/literature/CACTI-3DD_Architecture-level_modeling_for_3D_die-stacked_DRAM_main_memory.pdf)

## Get in touch

For technical support or questions regarding this repository: jakubandrzej.jastrzebski@polimi.it
