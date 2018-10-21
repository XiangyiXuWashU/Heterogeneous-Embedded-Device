
/****************************************************************************/
#ifndef __UPP_H__
#define __UPP_H__


// uPP Channel
#define uPP_CHA                    (0x00000000u)
#define uPP_CHB                    (0x00000001u)

#define uPP_DMA_CHI                (0x00000002u)
#define uPP_DMA_CHQ                (0x00000003u)

// uPP Pin Multiplexing

#define uPP_CHA_8BIT               (0x00000000u)
#define uPP_CHA_16BIT              (0x00000001u)
#define uPP_CHA_8BIT_CHB_8BIT      (0x00000002u)
#define uPP_CHA_8BIT_CHB_16BIT     (0x00000003u)
#define uPP_CHA_16BIT_CHB_8BIT     (0x00000004u)
#define uPP_CHA_16BIT_CHB_16BIT    (0x00000005u)

// uPP Data Alignment
#define uPP_DataPackingFmt_RJZE    (0x00000000u << 5)    // Right Alignment Zero Extension
#define uPP_DataPackingFmt_RJSE    (0x00000001u << 5)    // Right Alignment Symbol Extension
#define uPP_DataPackingFmt_LJZE    (0x00000002u << 5)    // Left Alignment Zero Extension

//Channel Width
#define uPP_DataPacking_FULL       (0x00000000u << 2)
#define uPP_DataPacking_9BIT       (0x00000001u << 2)
#define uPP_DataPacking_10BIT      (0x00000002u << 2)
#define uPP_DataPacking_11BIT      (0x00000003u << 2)
#define uPP_DataPacking_12BIT      (0x00000004u << 2)
#define uPP_DataPacking_13BIT      (0x00000005u << 2)
#define uPP_DataPacking_14BIT      (0x00000006u << 2)
#define uPP_DataPacking_15BIT      (0x00000007u << 2)

// Interface Width
#define uPP_InterfaceWidth_8BIT    (0x00000000u << 1)
#define uPP_InterfaceWidth_16BIT   (0x00000001u << 1)

// Data Rate
#define uPP_DataRate_SINGLE        (0x00000000u << 0)
#define uPP_DataRate_DOUBLE        (0x00000001u << 0)


// Double Data Rate MUX Mode
#define uPP_DDRDEMUX_DISABLE       (0x00000000u << 4)
#define uPP_DDRDEMUX_ENABLE        (0x00000001u << 4)

// Single Data Rate Staggered Mode
#define uPP_SDRTXIL_DISABLE        (0x00000000u << 3)
#define uPP_SDRTXIL_ENABLE         (0x00000001u << 3)

//Used Channel
#define uPP_CHN_ONE                (0x00000000u << 2)
#define uPP_CHN_TWO                (0x00000001u << 2)

//Running Mode
#define uPP_ALL_RECEIVE            (0x00000000u << 0)
#define uPP_ALL_TRANSMIT           (0x00000001u << 0)
#define uPP_DUPLEX0                (0x00000002u << 0)
#define uPP_DUPLEX1                (0x00000003u << 0)

// uPP Phase Pin Polarity
#define uPP_PIN_PHASE_NORMAL       (0x00000000u)
#define uPP_PIN_PHASE_INVERT       (0x00000001u)
#define uPP_PIN_TRIS               (0x00002000u)

#define uPP_PIN_WAIT               (0x00000020u)
#define uPP_PIN_ENABLE             (0x00000010u)
#define uPP_PIN_START              (0x00000008u)

#define uPP_PIN_POLARITY_WAIT      (0x00000004u)
#define uPP_PIN_POLARITY_ENABLE    (0x00000002u)
#define uPP_PIN_POLARITY_START     (0x00000001u)

// uPP Threshold
#define uPP_Threshold_64Bytes      (0x00000000u)
#define uPP_Threshold_128Bytes     (0x00000001u)
#define uPP_Threshold_256Bytes     (0x00000003u)

// uPP DMA Interruption
#define uPP_INT_EOL                (0x00000010u)
#define uPP_INT_EOW                (0x00000008u)
#define uPP_INT_ERR                (0x00000004u)
#define uPP_INT_UOR                (0x00000002u)
#define uPP_INT_DPE                (0x00000001u)

// uPP DMA
typedef struct
{
	unsigned int *WindowAddress;
	unsigned short LineCount;
	unsigned short ByteCount;
	unsigned short LineOffsetAddress;
} uPPDMAConfig;

//DMA Status
typedef struct
{
	unsigned int WindowAddress;
	unsigned short LineCount;
	unsigned short ByteCount;
	unsigned char FIFO;
	unsigned char PEND;
	unsigned char DMA;
} uPPDMAStatus;



void uPPPinMuxSetup(unsigned char OperatingMode);
void uPPClkConfig(unsigned int baseAddr, unsigned char channel, unsigned int Clk,
		            unsigned int moduleClk, unsigned char polarity);
void uPPEnable(unsigned int baseAdd);
void uPPDisable(unsigned int baseAdd);
void uPPPeripheralConfig(unsigned int baseAddr, unsigned int config);
void uPPDataFmtConfig(unsigned int baseAddr, unsigned char channel, unsigned int config);
void uPPChannelConfig(unsigned int baseAddr, unsigned int config);
void uPPPinConfig(unsigned int baseAddr, unsigned char channel, unsigned int config);
void uPPPinPolarityConfig(unsigned int baseAddr, unsigned char channel, unsigned int config);
void uPPIdleValueConfig(unsigned int baseAddr, unsigned char channel, unsigned int config);
void uPPThresholdConfig(unsigned int baseAddr, unsigned char channel, unsigned int config);
void uPPDLBConfig(unsigned int baseAddr, unsigned int config);

void uPPIntEnable(unsigned int baseAdd, unsigned char DMAchannel, unsigned int intFlags);
void uPPIntDisable(unsigned int baseAdd, unsigned char DMAchannel, unsigned int intFlags);
void uPPIntClear(unsigned int baseAdd, unsigned char DMAchannel, unsigned int intFlags);
unsigned int uPPIntStatus(unsigned int baseAdd, unsigned char DMAchannel);
void uPPEndOfInt(unsigned int baseAddr);

void uPPDMATransfer(unsigned int baseAdd, unsigned char DMAChannel, uPPDMAConfig *config);
uPPDMAStatus *uPPGetDMAStatus(unsigned int baseAdd, unsigned char channel);
void uPPReset(unsigned int baseAddr);

#endif
