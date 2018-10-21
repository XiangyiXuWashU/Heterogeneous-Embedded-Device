# Heterogeneous-Embedded-Device
Heterogeneous Embedded Device for Sensing Application
This device is optical signal DSP platform based on heterogeneous structure, which includes TI_OMAPL138 and Spartan_6_xc6slx9. The OMAP-L138 C6000 DSP+ARM processor is a low-power applications processor based on an ARM926EJ-S and a C674x DSP core. With the combination of FPGA+High speed ADC, this platform is capable of processing optical signal for different algorithms real timely. Meanwhile, a iOS app is developed to control the system wireless. 

1. TI DSP C6000: dsp
2. TI ARM9: host
3. Run shell: run
4. Shared file: shared


![image](https://user-images.githubusercontent.com/44279169/47262231-4e6a8c80-d4a8-11e8-8301-a77306a76241.png)


**Software Design**

- Embedded C (Programming language for OMAPL138)
- Objective C (Programming language for iOS APP)
- Verilog HDL (Programming language for FPGA)
- Linux (Embedded operating system)
- SYS/BIOS(TI DSP Embedded operating system )

**Hardware Design**

- Altium designer (Circuit design)
- OMAP-L138 (ARM926EJ-S + C6748)
- Spartan 6 xc6slx9 (FPGA)
