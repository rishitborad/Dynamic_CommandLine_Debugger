# @File																					\
	Version Information																	\
		makefile ver 2.0:	Refined version. Added dynmaic access to object files, 		\
							Improved elfagain and hex++ size by optimizations,			\
							Cleaned interface, Updated parser to support MAP files		\
							along with LST files and using a GUI for user file input	\
		makefile ver 1.0:	Initial version with post-build and symbol table generated	\
							from lst files. This is a semi-dynamic version as it need 	\
							some manual input the user.									\
																						\
	This makefile builds the files that are needed for enabling the debugging			\
	capabilities to an embedded	system platform that is implemented using C project		\
																						\
	This is a part of the Software based Debugger Tool offered to embedded 				\
	professionals as an open source software for using in their project.				\
																						\
	THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED			\
	OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF					\
	MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.		\
	I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR			\
	CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER. 									\
																						\
					You can contact the author at										\
				l e e p e n g 2 c h @ g m a i l . c o m									\
																						\
	@Note: The user must provide the path to the object files as the second makefile	\
	invoked in the post build step should be able to access all the object files during	\
	the linking process in the elfagain generation command.	Edit the macro OBJS	to		\
	specify the path to your directories where you are placing the object files			\

# Macro to remove files
RM := rm -rf

# Macros to directory paths

# This is the path to the project build folder where the lst/map/elf files are placed	\
  Note: Edit this path to the path on your machine where your _build/ directory is		\
  
BLDDIR		:= D:/Acads_Spring_16/CMPE_295B/Make_Source_Code/DCLD/_build

# This is same as BLDDIR but uses '_' instead of a forward slash '/' and ':' in path	\
  Note: Edit this to the path on your machine but just replace slashes and colon with	\
  the underscores. This serves for renaming the symbol names in the symbol_table object	\
  file that makes it easier to access.													\
  
OBJDIR		:= D__Acads_Spring_16_CMPE_295B_Make_Source_Code_DCLD__build_lpc1758_freertos

# This is the path to your python executable to facilitate the makefile able to invoke	\
  the python parser. Replace the path with the path of you python installation folder.	\
  Usually it will be in you main C drive. Also make sure you use PYTHON2.7 version		\
  Note: The path is separated by double forward slashes in this case					\
  
PYTDIR		:= C://Python27//python.exe

# These are the macros defined for naming the files
TARGET		:= lpc1758_freertos
HEX++		:= lpc1758_freertos_hex++

# Path to the initial build folder (if different replace)
BUILD_TARGET	:= $(BLDDIR)/$(TARGET)

# Path to the debug build folder (if different replace)
DEBUG_TARGET	:= $(BLDDIR)/$(TARGET)
HEX_EXE			:= $(BLDDIR)/$(HEX++)

# Macro for creating Symbol Table Object. "objcopy" is a utility function used to 		\
  convert file in one format (suppose binary) to another format (suppose elf) with 		\
  options to modify the contents of the target file										\
  
OBJCOPY		:= arm-none-eabi-objcopy

# Accessing the object files from initial build sweep. The object files are invidually	\
  accessed by providing the wildcard path to individual directories. Since there is no	\
  automatic way to grab the newly generated object files the user should include the	\
  newly added directory paths to this macro in order to access those object file		\
  Note: It is important that the order is preserved since the linker in dependent		\
  
OBJS		:= $(wildcard $(BLDDIR)/newlib/*.o) 				\
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

# Macros for each step in the debug_build process
SYMBOL_TABLE :=	$(DEBUG_TARGET).sym
SYMBOL_OBJ +=	$(DEBUG_TARGET).o
ELFAGAIN +=		$(DEBUG_TARGET).elfagain
HEXPLUSPLUS += 	$(HEX_EXE).hex

# Make all command
all: $(HEX_EXE).hex

# Generating lpc1758a_freertos.lst
# Note: We need to generate lst here because, all the secondary outputs are dependent	\
		on the post-build make. So we need it here to input to parser in debug build	\
		
$(BUILD_TARGET).lst: $(BUILD_TARGET).elf
	@echo 'Debugger: Generating listing file'
	arm-none-eabi-objdump --source --all-headers --demangle --line-numbers --wide $^ > $@
	@echo 'Finished building: $@'
	@echo ' '

# Rules to generate the symbol table and embed it and generate the final hex++ image
# Generating lpc1758_freertos.sym
$(DEBUG_TARGET).sym: $(BUILD_TARGET).lst
	@echo 'Debugger: Extracting symbol table from LST/MAP'
	$(PYTDIR) parse_symbol.py $@
	@rm map.txt
	@rm LUT.txt
	@echo 'Finished building: $@'
	@echo ' '

# Generating lpc1758_freertos.o
$(DEBUG_TARGET).o : $(DEBUG_TARGET).sym
	@echo 'Debugger: Converting symbol_table file to object file in ELF format'
	@cp $(<) $(DEBUG_TARGET).tmp
	@$(OBJCOPY) -I binary -O elf32-littlearm -B arm \
	--rename-section .data=.symbol_table,contents,alloc,load,readonly,data \
	--redefine-sym _binary_$(OBJDIR)_tmp_start=symbol_table \
	--redefine-sym _binary_$(OBJDIR)_tmp_end=symbol_table_end \
	--redefine-sym _binary_$(OBJDIR)_tmp_size=symbol_table_size_sym \
	$(DEBUG_TARGET).tmp $(@)
	@rm $(DEBUG_TARGET).tmp
	@echo 'Finished building: $@'
	@echo ' '

# Generating lpc1758_freertos.elfagain
$(DEBUG_TARGET).elfagain: $(DEBUG_TARGET).o
	@echo 'Debugger: Linking object files to generate elfagain'
	@echo Linking $^ with other obj files
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wshadow -Wlogical-op -Wfloat-equal -T ../loader.ld -nostartfiles -Wl,-Map,"$(BUILD_TARGET).map" -specs=nano.specs -o "$(DEBUG_TARGET).elfagain" $(DEBUG_TARGET).o $(OBJS)
	@echo 'Finished building: $@'
	@echo ' '

# Generating lpc1758_freertos_hex++.hex
$(HEX_EXE).hex: $(DEBUG_TARGET).elfagain
	@echo 'Debugger: Generating hex++ from elfagain'
	$(OBJCOPY) -O ihex $< $@
	@echo 'Finished building: $@'
	@echo ' '


# Other Targets
clean:
	-$(RM) $(SYMBOL_TABLE)$(SYMBOL_OBJ)$(ELFAGAIN)$(HEXPLUSPLUS)
	-@echo ' '

.PHONY: all clean
