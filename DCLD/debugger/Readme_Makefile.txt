Makefile_Readme

======================================X START OF EMBEDDING SYMBOL TABLE IN FLASH X======================================
BLDDIR		:= <Add the path to the target folder in your development environment> (for SJSUDev platform this is the "/_build" folder)

TARGET		:= lpc1758_helloworld
HEX++		:= lpc1758_freertos_hex++

BUILD_TARGET	:= $(BLDDIR)/$(TARGET)
DEBUG_TARGET	:= $(BLDDIR)/$(TARGET)
HEX_EXE			:= $(BLDDIR)/$(HEX++)

Step1:

Creating SYM table from an elf file:
	Macro:		PYTDIR := C://Python27//python.exe (Replace if the path is different on your PC)
	Dependency:	$(TARGET).elf
	Command:	$(PYTDIR) parse_symbol.py $@
	Notes:		This command invokes the parser script, which is a python based tool, that open up a GUI for the user to input the appropriate file. Based on the input file whether it is LST or MAP, repective parsing module will be activated and the symbols are extracted from the input file. The list of symbols are written on to a normal text file with extention ".sym".
	Remark:		The preferred input file is LST as we have perfomed extensive testing on LST compated to MAP.


Step2:

Generating the OBJECT file for the Symbol Table file:
	Macro:		OBJCOPY	:= arm-none-eabi-objcopy
	Dependency:	$(DEBUG_TARGET).sym
	Command:	@cp $(<) $(DEBUG_TARGET).tmp
				@$(OBJCOPY) -I binary -O elf32-littlearm -B arm \
				--rename-section .data=.symbol_table,contents,alloc,load,readonly,data \
				--redefine-sym _binary_$(OBJDIR)_tmp_start=symbol_table \
				--redefine-sym _binary_$(OBJDIR)_tmp_end=symbol_table_end \
				--redefine-sym _binary_$(OBJDIR)_tmp_size=symbol_table_size_sym \
				$(DEBUG_TARGET).tmp $(@)
				@rm $(DEBUG_TARGET).tmp
	Notes:		Here the sym file which is a binary file is being converted to the object file format. The object file format here is the arm-elf format. During the conversion we are renaming the section name ".data" to section ".symbol_table" and making a read-only section. This is done to place the symbol table into a separate sections the symbol_table section which is the sections, where the debugging symbol are saved to if the debugging option is enabled for a JTAG/SWD debugger". During this conversion we are also renaming some of the path symbols that will be added to "section_headers" in the ELF file.


Step3:

Generating the ELFAGAIN file by linking the symbol table object file with other object files generated during the primary build
	Macro:		OBJS		:= $(wildcard $(BLDDIR)/newlib/*.o) 				\
				$(wildcard $(BLDDIR)/debugger/src/*.o) 							\
				$(wildcard $(BLDDIR)/L5_Application/source/cmd_handlers/*.o) 	\
				$(wildcard $(BLDDIR)/L5_Application/source/*.o) 				\
				$(wildcard $(BLDDIR)/L5_Application/examples/*.o) 				\
				$(wildcard $(BLDDIR)/L5_Application/*.o) 						\
				$(wildcard $(BLDDIR)/L4_IO/wireless/src/*.o) 					\
				$(wildcard $(BLDDIR)/L4_IO/src/*.o) 							\
				$(wildcard $(BLDDIR)/L4_IO/fat/option/*.o) 						\
				$(wildcard $(BLDDIR)/L4_IO/fat/*.o) 							\
				$(wildcard $(BLDDIR)/L4_IO/fat/disk/*.o) 						\
				$(wildcard $(BLDDIR)/L3_Utils/tlm/src/*.o) 						\
				$(wildcard $(BLDDIR)/L3_Utils/src/*.o) 							\
				$(wildcard $(BLDDIR)/L2_Drivers/src/*.o) 						\
				$(wildcard $(BLDDIR)/L2_Drivers/base/*.o) 						\
				$(wildcard $(BLDDIR)/L1_FreeRTOS/src/*.o) 						\
				$(wildcard $(BLDDIR)/L1_FreeRTOS/portable/no_mpu/*.o) 			\
				$(wildcard $(BLDDIR)/L1_FreeRTOS/portable/*.o) 					\
				$(wildcard $(BLDDIR)/L1_FreeRTOS/hooks/*.o) 					\
				$(wildcard $(BLDDIR)/L0_LowLevel/source/*.o)
	Dependency:	$(DEBUG_TARGET).o
	Command:	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal \
				-T ../loader.ld -nostartfiles -Wl,-Map,"$(BUILD_TARGET).map" -specs=nano.specs -o "$(DEBUG_TARGET).elfagain" $(DEBUG_TARGET).o $(OBJS)
	Notes:		This step links all the object files to generate the ELF executable file. The linking is performed with the same command that the initial makefile is linking the object files. This is to preserve the same settings to the project which is like essentially using the same flag setting. This linking is slightly different from the primary linking as this will add a new section to the ELF file which is not present in the primary ELF, the .symbol_table section.
	Remark:		Here makefile cannot access the primary object files dynamically, so we need to provide paths to these files. Instead we privided a semi-dynamic way using the wildcards rather than giving manaul paths. you can refer to the intial makefile for the list of directories if you are using the eclipse IDE.
	Remark:		Also, the user have to disable the setting to ignoring of sections that weren't referenced in the code.


Step4:

Generating the final HEX++ fle
	Macro:		OBJCOPY	:= arm-none-eabi-objcopy
	Dependency:	$(DEBUG_TARGET).elfagain
	Command:	$(OBJCOPY) -O ihex $< $@
	Notes:		Once we have the secondary ELF file, it is a simple conversion from one format to another format. Here it is from ELF format to HEX format.
	
Step5:

Modifying the Loader file
	Commands:	.symbol_table : ALIGN(4)
			    {
			        __symbol_table_start__ = .;
			        *(.symbol_table)
			        __symbol_table_end__ = __symbol_table_start__ + SIZEOF(.symbol_table);
			    } > FLASH
	Notes:		The Loader.ld file which is used for the memory organization in the process should be modified to accomodate the SYMBOL_TABLE at the end of FLASH/ROM section.
	Remarks:	The loader file not only deals with the Flash memory but all other types of memory like RAM in a processor and places all the different sections like .text, .data, .bss etc., in their respective memory. Each of this memory contains information about specific types of source code information.

Step6:

Cleaning and Rebuilding the whole project.
	Macro:		SYMBOL_TABLE :=	$(DEBUG_TARGET).sym
				SYMBOL_OBJ +=	$(DEBUG_TARGET).o
				ELFAGAIN +=		$(DEBUG_TARGET).elfagain
				HEXPLUSPLUS += 	$(HEX_EXE).hex
	Command:	-$(RM) $(SYMBOL_TABLE)$(SYMBOL_OBJ)$(ELFAGAIN)$(HEXPLUSPLUS)
	Notes:		This cleans the files we have created when the user wants to clean the project


=======================================X END OF EMBEDDING SYMBOL TABLE IN FLASH X=======================================


==========================X START OF SETTING UP POST BUILD OPTION FOR EMBEDDING SYMBOL TABLE X==========================

In eclipse, we can configure the build process to perform some more commands or generate some more files as per our needs before/after the inital build has been completed. Before the build is 'pre-build' and after the build is 'post-build'

This is specifed in C/C++ build -> build steps -> pre/post build. These are general commands that any makefile contains.

In our project, we use both pre-build and post-build options.
- Pre-build:	To clean the build files from the debugger directory
- Post-build: 	To build the debugger build files necessary to allow debugging.

Step:	Pre-build
Purpose:	Clean Existing Symbol_Table files
Files:		../debugger/makefile
Command:	$(MAKE) -C ../lpc1758_freertos/debugger clean (or) cd ../lpc1758_freertos/debugger $(MAKE) clean
Note:		This command checks for makefile in the given directory and calls in 'make clean' that performs the clean operation specified in the
			makefile.

Step:	Post-build
Purpose:	Generating and embedding symbol table
Files:		../debugger/makefile
Command:	$(MAKE) -C ../lpc1758_freertos/debugger (or) cd ../lpc1758_freertos/debugger $(MAKE)
Note:		This command checks for makefile in the given directory and calls in 'make all' that performs the build all operation as specified
			in the makefile.

Step:	Post-build with multiple commands
Purpose:	Generating Symbol and Hex++ and Create binary file
Files:		../debugger/makefile
Command:	$(MAKE) -C ../lpc1758_freertos/debugger && arm-none-eabi-objcopy -O binary ${ProjName}.elf ${ProjName}.bin
Note:		This command builds the symbol table files and also generates some additional necessary files like binary file in this case.
			The symbol '&&' is used to join multiple commands to the post-build operation.


===========================X END OF SETTING UP POST BUILD OPTION FOR EMBEDDING SYMBOL TABLE X===========================