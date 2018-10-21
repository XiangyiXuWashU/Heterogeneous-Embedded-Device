################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
configPkg/linker.cmd: ../dsp.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_25_03_72/xs" --xdcpath="C:/ti/bios_6_35_04_50/packages;C:/ti/ccsv5/ccs_base;Z:/MINILOC/OMAPL138/shared;Z:/LIB/ipc_1_25_03_15/packages;Z:/LIB/syslink_2_21_01_05/packages;Z:/LIB/dsplib_c674x_3_1_1_1/packages;Z:/LIB/dsplib_c674x_3_1_1_1/inc;Z:/LIB/mathlib_c674x_3_0_2_0/packages;Z:/LIB/mathlib_c674x_3_0_2_0/inc;Z:/LIB/OMAPL138_StarterWare_1_10_04_01/include;Z:/LIB/OMAPL138_StarterWare_1_10_04_01/include/hw;Z:/LIB/OMAPL138_StarterWare_1_10_04_01/drivers;" xdc.tools.configuro -o configPkg -t ti.targets.elf.C674 -p miniloc -r debug -c "C:/ti/ccsv5/tools/compiler/c6000_7.4.4" --compileOptions "-g --optimize_with_debug" "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/compiler.opt: | configPkg/linker.cmd
configPkg/: | configPkg/linker.cmd

main_dsp.obj: ../main_dsp.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C6000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c6000_7.4.4/bin/cl6x" -mv6740 --abi=eabi -g --include_path="C:/ti/ccsv5/tools/compiler/c6000_7.4.4/include" --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="main_dsp.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


