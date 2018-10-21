################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/c6000_7.4.4

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./app/queue.obj" \
"./app/message_loop.obj" \
"./app/ad_workshop.obj" \
"./codec/fir.obj" \
"./codec/fft.obj" \
"./driver/uPP_Config.obj" \
"./driver/setgpio.obj" \
"./driver/ring_buffer.obj" \
"./driver/ad9238.obj" \
"./ipc/message_heap.obj" \
"./ipc/ipc_ring_buffer.obj" \
"./main_dsp.obj" \
"../uPP.lib" \
-l"Z:\LIB\OMAPL138_StarterWare_1_10_04_01\binary\c674x\cgt\omapl138\drivers\Release\drivers.lib" \
-l"Z:\LIB\OMAPL138_StarterWare_1_10_04_01\binary\c674x\cgt\utils\Release\utils.lib" \
-l"Z:\LIB\mathlib_c674x_3_0_2_0\lib\mathlib.ae674" \
-l"Z:\LIB\dsplib_c674x_3_1_1_1\lib\dsplib.ae674" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include ipc/subdir_vars.mk
-include driver/subdir_vars.mk
-include codec/subdir_vars.mk
-include app/subdir_vars.mk
-include subdir_rules.mk
-include ipc/subdir_rules.mk
-include driver/subdir_rules.mk
-include codec/subdir_rules.mk
-include app/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: MINILOC.out

# Tool invocations
MINILOC.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C6000 Linker'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6740 --abi=eabi -g --display_error_number --diag_warning=225 --diag_wrap=off -z -m"MINILOC.map" -i"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/lib" -i"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="MINILOC_linkInfo.xml" --rom_model -o "MINILOC.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(C6000_EXECUTABLE_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(GEN_OPTS__QUOTED)$(GEN_CMDS__QUOTED) "MINILOC.out"
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-$(RM) "main_dsp.pp" "ipc\ipc_ring_buffer.pp" "ipc\message_heap.pp" "driver\ad9238.pp" "driver\ring_buffer.pp" "driver\setgpio.pp" "driver\uPP_Config.pp" "codec\fft.pp" "codec\fir.pp" "app\ad_workshop.pp" "app\message_loop.pp" "app\queue.pp" 
	-$(RM) "main_dsp.obj" "ipc\ipc_ring_buffer.obj" "ipc\message_heap.obj" "driver\ad9238.obj" "driver\ring_buffer.obj" "driver\setgpio.obj" "driver\uPP_Config.obj" "codec\fft.obj" "codec\fir.obj" "app\ad_workshop.obj" "app\message_loop.obj" "app\queue.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

