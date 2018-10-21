#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/bios_6_35_04_50/packages;C:/ti/ccsv5/ccs_base;Z:/MINILOC/OMAPL138/shared;Z:/LIB/ipc_1_25_03_15/packages;Z:/LIB/syslink_2_21_01_05/packages;Z:/LIB/dsplib_c674x_3_1_1_1/packages;Z:/LIB/dsplib_c674x_3_1_1_1/inc;Z:/LIB/mathlib_c674x_3_0_2_0/packages;Z:/LIB/mathlib_c674x_3_0_2_0/inc;Z:/LIB/OMAPL138_StarterWare_1_10_04_01/include;Z:/LIB/OMAPL138_StarterWare_1_10_04_01/include/hw;Z:/LIB/OMAPL138_StarterWare_1_10_04_01/drivers
override XDCROOT = C:/ti/xdctools_3_25_03_72
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/bios_6_35_04_50/packages;C:/ti/ccsv5/ccs_base;Z:/MINILOC/OMAPL138/shared;Z:/LIB/ipc_1_25_03_15/packages;Z:/LIB/syslink_2_21_01_05/packages;Z:/LIB/dsplib_c674x_3_1_1_1/packages;Z:/LIB/dsplib_c674x_3_1_1_1/inc;Z:/LIB/mathlib_c674x_3_0_2_0/packages;Z:/LIB/mathlib_c674x_3_0_2_0/inc;Z:/LIB/OMAPL138_StarterWare_1_10_04_01/include;Z:/LIB/OMAPL138_StarterWare_1_10_04_01/include/hw;Z:/LIB/OMAPL138_StarterWare_1_10_04_01/drivers;C:/ti/xdctools_3_25_03_72/packages;..
HOSTOS = Windows
endif
