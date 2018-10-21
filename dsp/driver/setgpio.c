#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../dsp/driver/setgpio.h"

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Registry.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/family/c64p/Cache.h>

#include "hw/hw_types.h"                          //Macros
#include "hw/hw_syscfg0_OMAPL138.h"       //Hardware definitions for OMAPL138
#include "hw/hw_psc_OMAPL138.h"            //Hardware definitions for OMAPL138
#include "hw/soc_OMAPL138.h"                 //peripheral information for OMAPL138 SOC
#include "psc.h"                                      //Power and Sleep module control
#include "gpio.h"


#define PINMUX1_GPIO0_0_ENABLE    (SYSCFG_PINMUX1_PINMUX1_31_28_GPIO0_0  << \
                                    SYSCFG_PINMUX1_PINMUX1_31_28_SHIFT)
#define PINMUX1_GPIO0_1_ENABLE    (SYSCFG_PINMUX1_PINMUX1_27_24_GPIO0_1  << \
                                    SYSCFG_PINMUX1_PINMUX1_27_24_SHIFT)
#define PINMUX1_GPIO0_2_ENABLE    (SYSCFG_PINMUX1_PINMUX1_23_20_GPIO0_2  << \
                                    SYSCFG_PINMUX1_PINMUX1_23_20_SHIFT)
#define PINMUX1_GPIO0_5_ENABLE    (SYSCFG_PINMUX1_PINMUX1_11_8_GPIO0_5  << \
                                    SYSCFG_PINMUX1_PINMUX1_11_8_SHIFT)
#define PINMUX1_GPIO0_6_ENABLE    (SYSCFG_PINMUX1_PINMUX1_7_4_GPIO0_6  << \
                                    SYSCFG_PINMUX1_PINMUX1_7_4_SHIFT)

//Used for Sampling Rate Control Bits
#define PINMUX18_GPIO8_14_ENABLE    (SYSCFG_PINMUX18_PINMUX18_15_12_GPIO8_14  << \
                                    SYSCFG_PINMUX18_PINMUX18_15_12_SHIFT)
#define PINMUX18_GPIO8_13_ENABLE    (SYSCFG_PINMUX18_PINMUX18_19_16_GPIO8_13  << \
                                    SYSCFG_PINMUX18_PINMUX18_19_16_SHIFT)
#define PINMUX18_GPIO8_12_ENABLE    (SYSCFG_PINMUX18_PINMUX18_23_20_GPIO8_12  << \
                                    SYSCFG_PINMUX18_PINMUX18_23_20_SHIFT)
#define PINMUX18_GPIO8_15_ENABLE    (SYSCFG_PINMUX18_PINMUX18_11_8_GPIO8_15  << \
                                    SYSCFG_PINMUX18_PINMUX18_11_8_SHIFT)
#define PINMUX4_GPIO1_5_ENABLE    (SYSCFG_PINMUX4_PINMUX4_11_8_GPIO1_5  << \
                                    SYSCFG_PINMUX4_PINMUX4_11_8_SHIFT)

//Used for Channel Select
#define PINMUX4_GPIO1_4_ENABLE    (SYSCFG_PINMUX4_PINMUX4_15_12_GPIO1_4  << \
                                    SYSCFG_PINMUX4_PINMUX4_15_12_SHIFT)


void GPIOBank0Pin0PinMuxSetup(void)
{
        unsigned int savePinmux = 0;
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) &
                           ~(SYSCFG_PINMUX1_PINMUX1_31_28));
        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) =
                   (PINMUX1_GPIO0_0_ENABLE | savePinmux);
}

void GPIOBank0Pin1PinMuxSetup(void)
{
        unsigned int savePinmux = 0;
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) &
                           ~(SYSCFG_PINMUX1_PINMUX1_27_24));
        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) =
                   (PINMUX1_GPIO0_1_ENABLE | savePinmux);
}

void GPIOBank0Pin2PinMuxSetup(void)
{
        unsigned int savePinmux = 0;
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) &
                           ~(SYSCFG_PINMUX1_PINMUX1_23_20));
        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) =
                   (PINMUX1_GPIO0_2_ENABLE | savePinmux);
}

void GPIOBank0Pin5PinMuxSetup(void)
{
        unsigned int savePinmux = 0;
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) &
                           ~(SYSCFG_PINMUX1_PINMUX1_11_8));
        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) =
                   (PINMUX1_GPIO0_5_ENABLE | savePinmux);
}

void GPIOBank0Pin6PinMuxSetup(void)
{
    unsigned int savePinmux = 0;
    savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) &
                       ~(SYSCFG_PINMUX1_PINMUX1_7_4));
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) =
               (PINMUX1_GPIO0_6_ENABLE | savePinmux);
}

//Sampling Rate Control Bit[0]
void GPIOBank8Pin14PinMuxSetup(void)
{
    unsigned int savePinmux = 0;
    savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) &
                       ~(SYSCFG_PINMUX18_PINMUX18_15_12));
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) =
               (PINMUX18_GPIO8_14_ENABLE | savePinmux);
}

//Sampling Rate Control Bit[1]
void GPIOBank8Pin13PinMuxSetup(void)
{
    unsigned int savePinmux = 0;
    savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) &
                       ~(SYSCFG_PINMUX18_PINMUX18_19_16));
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) =
               (PINMUX18_GPIO8_13_ENABLE | savePinmux);
}

//Sampling Rate Control Bit[2]
void GPIOBank8Pin12PinMuxSetup(void)
{
    unsigned int savePinmux = 0;
    savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) &
                       ~(SYSCFG_PINMUX18_PINMUX18_23_20));
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) =
               (PINMUX18_GPIO8_12_ENABLE | savePinmux);
}

//Sampling Rate Control Bit[3]
void GPIOBank8Pin15PinMuxSetup(void)
{
    unsigned int savePinmux = 0;
    savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) &
                       ~(SYSCFG_PINMUX18_PINMUX18_11_8));
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) =
               (PINMUX18_GPIO8_15_ENABLE | savePinmux);
}

//Sampling Rate Control Bit[4]
void GPIOBank1Pin5PinMuxSetup(void)
{
    unsigned int savePinmux = 0;
    savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(4)) &
                       ~(SYSCFG_PINMUX4_PINMUX4_11_8));
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(4)) =
               (PINMUX4_GPIO1_5_ENABLE | savePinmux);
}


//Channel Control
void GPIOBank1Pin4PinMuxSetup(void)
{
    unsigned int savePinmux = 0;
    savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(4)) &
                       ~(SYSCFG_PINMUX4_PINMUX4_15_12));
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(4)) =
               (PINMUX4_GPIO1_4_ENABLE | savePinmux);
}


void GPIOInit(void)
{

    // OMAPL138 and  DSP C6748 have 144  GPIO
    // Range 1-144
	// GPIO0[0] 1
    // GPIO1[0] 17
	// GPIO2[0] 33
    // GPIO3[0] 49
	// GPIO4[0] 65
    // GPIO5[0] 81
	// GPIO6[0] 97
	// GPIO7[0] 113
	// GPIO8[0] 129

	PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_GPIO, PSC_POWERDOMAIN_ALWAYS_ON, PSC_MDCTL_NEXT_ENABLE);

    //Configure GPIO general input and output
	GPIOBank0Pin0PinMuxSetup();
	GPIOBank0Pin1PinMuxSetup();
	GPIOBank0Pin2PinMuxSetup();
	GPIOBank0Pin5PinMuxSetup();

	//Configure GPIO Direction
    GPIODirModeSet(SOC_GPIO_0_REGS, 1, GPIO_DIR_OUTPUT);    //D5 LED1    GPIO0[0]
    GPIODirModeSet(SOC_GPIO_0_REGS, 7, GPIO_DIR_OUTPUT);    //IO1           GPIO0[6]
    GPIODirModeSet(SOC_GPIO_0_REGS, 2, GPIO_DIR_OUTPUT);    //IO2           GPIO0[1]
    GPIODirModeSet(SOC_GPIO_0_REGS, 3, GPIO_DIR_OUTPUT);    //D6  LED2    GPIO0[2]
}

void SampleRateChannelSelectGPIOInit(void)
{

    // OMAPL138 and  DSP C6748 have 144  GPIO
    // Range 1-144
	// GPIO0[0] 1
    // GPIO1[0] 17
	// GPIO2[0] 33
    // GPIO3[0] 49
	// GPIO4[0] 65
    // GPIO5[0] 81
	// GPIO6[0] 97
	// GPIO7[0] 113
	// GPIO8[0] 129

	PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_GPIO, PSC_POWERDOMAIN_ALWAYS_ON, PSC_MDCTL_NEXT_ENABLE);


	//Sampling Rate Control Bit
	GPIOBank8Pin13PinMuxSetup();
	GPIOBank8Pin12PinMuxSetup();
	GPIOBank8Pin15PinMuxSetup();
	GPIOBank1Pin5PinMuxSetup();

	//Channel Select
	GPIOBank1Pin4PinMuxSetup();

	//Configure GPIO Direction
    GPIODirModeSet(SOC_GPIO_0_REGS, 142, GPIO_DIR_OUTPUT);//UPP_CHB_ENABLE       Sampling Rate Control Bit[0]
    GPIODirModeSet(SOC_GPIO_0_REGS, 141, GPIO_DIR_OUTPUT);//UPP_CHB_WAIT          Sampling Rate Control Bit[1]
    GPIODirModeSet(SOC_GPIO_0_REGS, 144, GPIO_DIR_OUTPUT);//UPP_CHB_CLK            Sampling Rate Control Bit[2]
    GPIODirModeSet(SOC_GPIO_0_REGS, 22, GPIO_DIR_OUTPUT); //I2C0_SCL/SPI1_SCSn7  Sampling Rate Control Bit[3]

    GPIODirModeSet(SOC_GPIO_0_REGS, 21, GPIO_DIR_OUTPUT); //I2C0_SDA/SPI1_SCSn6  Channel Select
}


void switchLED1(int status)
{
	if(status)
	{
		GPIOPinWrite(SOC_GPIO_0_REGS, 1, GPIO_PIN_HIGH); //LED1    GPIO0[0]
	}
	else
	{
		GPIOPinWrite(SOC_GPIO_0_REGS, 1, GPIO_PIN_LOW); //LED1    GPIO0[0]
	}
}

void switchLED2(int status)
{
	if(status)
	{
		GPIOPinWrite(SOC_GPIO_0_REGS, 3, GPIO_PIN_HIGH); //LED2    GPIO0[3]
	}
	else
	{
		GPIOPinWrite(SOC_GPIO_0_REGS, 3, GPIO_PIN_LOW); //LED2    GPIO0[3]
	}
}

//Select ADC Channel_A=0 Channel_B=1
void selectADCChannel(int channel)
{
	if(channel)
	{
		GPIOPinWrite(SOC_GPIO_0_REGS, 21, GPIO_PIN_HIGH); //I2C0_SDA/SPI1_SCSn6  Channel Select
	}
	else
	{
		GPIOPinWrite(SOC_GPIO_0_REGS, 21, GPIO_PIN_LOW); //I2C0_SDA/SPI1_SCSn6  Channel Select
	}
}

//Sampling Rate Bit Control
void sampleRateControl( int bit3, int bit2, int bit1, int bit0)
{
	if(bit3) GPIOPinWrite(SOC_GPIO_0_REGS, 22, GPIO_PIN_HIGH);
	else GPIOPinWrite(SOC_GPIO_0_REGS, 22, GPIO_PIN_LOW);

	if(bit2) GPIOPinWrite(SOC_GPIO_0_REGS, 144, GPIO_PIN_HIGH);
	else GPIOPinWrite(SOC_GPIO_0_REGS, 144, GPIO_PIN_LOW);

	if(bit1) GPIOPinWrite(SOC_GPIO_0_REGS, 141, GPIO_PIN_HIGH);
	else GPIOPinWrite(SOC_GPIO_0_REGS, 141, GPIO_PIN_LOW);

	if(bit0) GPIOPinWrite(SOC_GPIO_0_REGS, 142, GPIO_PIN_HIGH);
	else GPIOPinWrite(SOC_GPIO_0_REGS, 142, GPIO_PIN_LOW);

}

