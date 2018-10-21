#include <stdlib.h>
#include <string.h>

#include "../dsp/driver/ad9238.h"

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Assert.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Clock.h>

#include "hw/hw_types.h"                             //Macros
#include "hw/hw_syscfg0_OMAPL138.h"       //Hardware definitions for OMAPL138
#include "hw/hw_psc_OMAPL138.h"             // Hardware definitions for OMAPL138
#include "hw/soc_OMAPL138.h"                   //peripheral information for OMAPL138 SOC
#include "c674x/omapl138/interrupt.h"        //DSP interrupt related API
#include "gpio.h"                                         //Abstraction layer for GPIO and some related macros
#include "hw_upp.h"                                   //uPP register definitions
#include "upp.h"                                         //Abstraction layer for uPP
#include "psc.h"                                         //Power and Sleep module control
#include "../dsp/driver/setgpio.h"
#include "uPP_Config.h"

//Macros Define


//Global variable
volatile int upp_interrupt_count = 0;
volatile int upp_error_count = 0;


//uPP Interruption
Void uPPIsr(UArg arg)
{
	unsigned int intr_dmai_status;

	intr_dmai_status = uPPIntStatus(SOC_UPP_0_REGS, uPP_DMA_CHI);

	while(intr_dmai_status != 0)
	{
		if (intr_dmai_status & uPP_INT_EOL)
		{
			uPPIntClear(SOC_UPP_0_REGS, uPP_DMA_CHI, uPP_INT_EOL);
		}

		if (intr_dmai_status & uPP_INT_EOW)
		{
			uPPIntClear(SOC_UPP_0_REGS, uPP_DMA_CHI, uPP_INT_EOW);
			upp_interrupt_count++;
		}

		if (intr_dmai_status & uPP_INT_ERR)
		{
			uPPIntClear(SOC_UPP_0_REGS, uPP_DMA_CHI, uPP_INT_ERR);
			upp_error_count++;
		}

		if (intr_dmai_status & uPP_INT_UOR)
		{
			uPPIntClear(SOC_UPP_0_REGS, uPP_DMA_CHI, uPP_INT_UOR);
			upp_error_count++;
		}

		if (intr_dmai_status & uPP_INT_DPE)
		{
			uPPIntClear(SOC_UPP_0_REGS, uPP_DMA_CHI, uPP_INT_DPE);
			upp_error_count++;
		}


		intr_dmai_status = uPPIntStatus(SOC_UPP_0_REGS, uPP_DMA_CHI);

	}

	uPPEndOfInt(SOC_UPP_0_REGS);

}

//uPP Initialization
void OmaplFpgauPPSetup(void)
{
	 //Enable PSC_UPP
	PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_UPP, PSC_POWERDOMAIN_ALWAYS_ON, PSC_MDCTL_NEXT_ENABLE);

	 //Pin multiplexing configuration
	uPPPinMuxSetup(uPP_CHA_16BIT);

	 //Reset uPP
	uPPReset(SOC_UPP_0_REGS);

	 //Data format
	uPPDataFmtConfig(SOC_UPP_0_REGS, uPP_CHA, uPP_DataPackingFmt_LJZE | uPP_DataPacking_FULL
							| uPP_InterfaceWidth_16BIT |uPP_DataRate_SINGLE);

	 //Channel Configuration
	uPPChannelConfig(SOC_UPP_0_REGS, uPP_DDRDEMUX_DISABLE | uPP_SDRTXIL_DISABLE | uPP_CHN_ONE
							| uPP_ALL_RECEIVE );

	 //Pin configuration
	uPPPinConfig(SOC_UPP_0_REGS, uPP_CHA,  uPP_PIN_START| uPP_PIN_ENABLE | uPP_PIN_WAIT);

	 //Interruption enable
	uPPIntEnable(SOC_UPP_0_REGS, uPP_DMA_CHI,  uPP_INT_EOW);

	 //uPP enable
	uPPEnable(SOC_UPP_0_REGS);

	//Use uPP CHB Start Wait Enable Clock and I2C_SCL Pins for sampling rate control
	//Initialize the SampleRate and Channel Select GPIO
	SampleRateChannelSelectGPIOInit();

    //Init the ADC Channel Select
	//Swithc to CHB
    selectADCChannel(1);

    //Init the Sampling Rate Control Bit
    //Rate18=50MHz
    sampleRateControl(1, 1, 0, 0);

}

//Register uPP interruption
void uPPIntRegister()
{
    Hwi_Params hwiParams;
	Error_Block eb;
	Error_init(&eb);
    Hwi_Params_init(&hwiParams);
    hwiParams.eventId = SYS_INT_UPP_INT;
	hwiParams.arg = NULL;
    hwiParams.enableInt = FALSE;

    if (Hwi_create(C674X_MASK_INT8, uPPIsr, &hwiParams, &eb) == NULL)
        {
          Log_error0("Fail to create uPPIsr");
        }

	Hwi_enableInterrupt(C674X_MASK_INT8);
}

















