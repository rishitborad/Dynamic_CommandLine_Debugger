@File
This file describes the steps to follow to use this debugger. These steps are specific to the SJSUDev platform in some contexts.

Step0:	Modify the current project linker settings to inlcude unused sections in the ELF file to facilitate including the .symbol_table section.
		Go to your project > Properties > C/C++ Build > Settings > Tool Setting > Cross ARM C++ Linker > General
		Uncheck the option "Remove unused sections (-Xlinker --gc-sections)" option and click apply and OK.

Step1:	Download the makefile, loader.ld and parse_symbol.py and the rest of source code files. Place the makefile, parser_symbol.py and source files in a separate folder in the 			project. Maybe create some new folder for the debugger files.

Step2:	In makefile, verify the path to your build directories. If the path is different from the given path, replace the current path in "BLDDIR" with whatever directory your "../		SJSU_Dev/lpc1758_freertos/_build" is present.

Step3:	Repeat step2 for the "OBJDIR". But replace the forward slash and colon in path with an underscore. Look at the comment in makefile if not understood.

Step4:	In makefile, verify the path to your python directory. If you have a different setup for your python libraries replace the path in "PYTDIR" with your directory path. Rest 			all other paths are relative to these 2 paths, so there shouldn't be any issue.

Note:	The python parser and the makefile should be in the same directory.

Step5:	Now go to your project > Properties > C/C++ Build > Settings

Step6:	Go to the tab Build Steps and in here add the following command in the command text box
		$(MAKE) -C <replace-path-to-this-directory>/lpc1758_freertos/L6_Debugger && arm-none-eabi-objcopy -O binary ${ProjName}.elf ${ProjName}.bin

Step7:	Add any comment to the description text box like
		Post-build: Generating Symbol and Hex++ and Create binary file

Step8:	Go to the tab Tool Settings and in here go to the sections Cross ARM C Compiler and Cross ARM C++ Compiler to include the debugger folder in the Includes option.

Step9:	Click OK and that closes the properties....

Step10:	Replace the linker script by replacing the existing loader.ld with the given loader.ld file.

Finish: Here you go you are all set to build the HEX++ from the makefile in post-build