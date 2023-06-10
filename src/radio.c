/*! @file radio.c
 * @brief This file contains functions to interface with the radio chip.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#include "includes.h"
#include "spi.h"
#include "si446x_cmd.h"
#include "si446x_api_lib.h"
#include "radio.h"
#include "radio_config.h"
#include <string.h>
/*****************************************************************************
 *  Local Macros & Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Global Variables
 *****************************************************************************/
/*
const SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8, SEG_CODE) = \
             RADIO_CONFIGURATION_DATA_ARRAY;

const SEGMENT_VARIABLE(RadioConfiguration, tRadioConfiguration, SEG_CODE) = \
                       RADIO_CONFIGURATION_DATA;

*/
const U8 Radio_Configuration_Data_Array[] = RADIO_CONFIGURATION_DATA_ARRAY;

/*
const SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration, SEG_CODE, SEG_CODE) = \
                   &RadioConfiguration;
*/

// SEGMENT_VARIABLE(customRadioPacket[RADIO_MAX_PACKET_LENGTH], U8, SEG_XDATA);
// extern void vRadio_StartRX(U8 channel);

#define MaxRadioPacketLenth 256
unsigned char fixRadioPacket[MaxRadioPacketLenth];
// extern unsigned char WirelessA2Buf[64];
unsigned char TXSending;
unsigned char RadioPacketLength = 0;
uint8_t g_rssi_value = 0;
//#define MaxRadioPacketLenth 64
#define WakeupFrame 1
//#define RadioTxWakeuoPacketLength 7317//7296+21
#define RadioTxWakeuoPacketLength 7552 // 64+156*48+4+17
#define RADIO_TX_ALMOST_EMPTY_THRESHOLD 48
U8 TxType = 0;
U16 NumOfRemainBytes = 0;
U16 NumOfHadTxBytes = 0;
const uint8_t CustomTxSyncWord[4] = {0xD3, 0x91, 0xD3, 0x91};
const uint8_t StandardTxSyncWord[4] = {0xCB, 0x89, 0xCB, 0x89};
const uint8_t CustomLongTxSyncWord[4] = {0x55, 0x55, 0x55, 0x55};
unsigned char fixRadioPacket[MaxRadioPacketLenth];
unsigned char fixRadioTxPacket[MaxRadioPacketLenth] = {0x10, 0x10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
unsigned char fixRadioTxPacketLength = 0;
#define RF_SYNC_CONFIG_CUSTOM 0x11, 0x06, 0x00, 0x03, 0xCB, 0x89, 0xCB, 0x89, 0x00
//_Dev Dev;
/*****************************************************************************
 *  Local Function Declarations
 *****************************************************************************/
void vRadio_PowerUp(void);
/*
void ReadSi4460Id(void)
{
U16 u16Part_info = 0;
U8 u8ChipRev = 0;
    si446x_part_info();
    u16Part_info = Si446xCmd.PART_INFO.PART;
    u8ChipRev = Si446xCmd.PART_INFO.CHIPREV;
    if(u16Part_info==0x4460)
    {
            u16Part_info = Si446xCmd.PART_INFO.PART;
            u8ChipRev = Si446xCmd.PART_INFO.CHIPREV;
    }
}
*/
/*!
 *  Power up the Radio.
 *
 *  @note
 *
 */
void vRadio_PowerUp(void)
{
    // SEGMENT_VARIABLE(wDelay,  U16, SEG_XDATA) = 0u;
    U16 wDelay = 0;
    /* Hardware reset the chip */
    si446x_reset();

    /* Wait until reset timeout or Reset IT signal */
    // for (; wDelay < pRadioConfiguration->Radio_Delay_Cnt_After_Reset; wDelay++);
    for (; wDelay < 0x1FFF; wDelay++)
        ;
}

/*!
 *  Radio Initialization.
 *
 *  @author Sz. Papp
 *
 *  @note
 *
 */

U16 u16Part_info = 0;
U8 u8ChipRev = 0;




void vRadio_Init(void)
{
    U16 wDelay;
    InitSi446xSPI();

    Clr_GPIO_PWRDN();
    Set_GPIO_SCS();
    
    /* Power Up the radio chip */
    vRadio_PowerUp();

    u16Part_info = 0;
    u8ChipRev = 0;
	/* 读取part信息 */
    si446x_part_info();
    u8ChipRev = Si446xCmd.PART_INFO.CHIPREV;
    u16Part_info = Si446xCmd.PART_INFO.PART;

    /* Load radio configuration */
    while (SI446X_SUCCESS != si446x_configuration_init(Radio_Configuration_Data_Array))
    {
        for (wDelay = 0x7FFF; wDelay--;)
            ;
        /* Power Up the radio chip */
        vRadio_PowerUp();
    }
    
    // Read ITs, clear pending ones
    si446x_get_int_status(0u, 0u, 0u);
    u16Part_info = 0;
    u8ChipRev = 0;
    si446x_part_info();
    u8ChipRev = Si446xCmd.PART_INFO.CHIPREV;
    u16Part_info = Si446xCmd.PART_INFO.PART;

    vRadio_StartRX(0, 0);
}

typedef struct rf_freCmd{
    uint8_t len;//数据长度
    uint8_t command;//命令
    uint8_t group_id;//组id
    uint8_t number;//数量
    uint8_t start_id;//起始id
    uint8_t inite;//7bit,fc_{inite}
    uint8_t frac[3];//20bit,fc_{frac}
    uint8_t endflag;
}rf_freCmd;
/* 默认868.26 */
static rf_freCmd rf_freCMD = {
    .len = 0x08,
    .command = 0x11,
    .group_id = 0x40,
    .number = 4,
    .start_id = 0,
    .inite = 0x38,
    .frac[0] = 0x0F,
    .frac[1] = 0x12,
    .frac[2] = 0x6e,
    .endflag = 0x00,
};
static float last_frequence = 0;
void vRadio_Init_with_fre(float frequence)
{
    U16 wDelay;
    float fre_seed = 0;
    uint32_t fre_frac_seed = 0;
    InitSi446xSPI();

    Clr_GPIO_PWRDN();
    Set_GPIO_SCS();
    
    /* Power Up the radio chip */
    vRadio_PowerUp();

    u16Part_info = 0;
    u8ChipRev = 0;
	/* 读取part信息 */
    si446x_part_info();
    u8ChipRev = Si446xCmd.PART_INFO.CHIPREV;
    u16Part_info = Si446xCmd.PART_INFO.PART;

    /* Load radio configuration */
    while (SI446X_SUCCESS != si446x_configuration_init(Radio_Configuration_Data_Array))
    {
        for (wDelay = 0x7FFF; wDelay--;)
            ;
        /* Power Up the radio chip */
        vRadio_PowerUp();
    }
    /* 修改频率设置 */
    if (last_frequence != frequence)
    {
        last_frequence = frequence;
        /*  RF_fre = (fc_{inite} + (fc_{frac}/2^19)) * 15  
            PS:15是一个常数，和硬件有关，我们的板子是15
            fc_{frac}/2^19 的值需要在1到2之间，所以inite计算的时候-1。
        */
        fre_seed = frequence/15.0;
        rf_freCMD.inite = ((uint8_t)fre_seed) - 1;
        fre_frac_seed = (fre_seed - (float)rf_freCMD.inite) * 524288;
        rf_freCMD.frac[0] = fre_frac_seed >> 16;
        rf_freCMD.frac[1] = fre_frac_seed >> 8;
        rf_freCMD.frac[2] = fre_frac_seed;
    }
    
    while (SI446X_SUCCESS != si446x_configuration_init((uint8_t *)&rf_freCMD))
    {
        for (wDelay = 0x7FFF; wDelay--;)
            ;
        /* Power Up the radio chip */
        vRadio_PowerUp();
    }
    
    // Read ITs, clear pending ones
    si446x_get_int_status(0u, 0u, 0u);
    u16Part_info = 0;
    u8ChipRev = 0;
    si446x_part_info();
    u8ChipRev = Si446xCmd.PART_INFO.CHIPREV;
    u16Part_info = Si446xCmd.PART_INFO.PART;

    vRadio_StartRX(0, 0);
}

extern void radio_hal_ClearNsel(void);
extern void radio_hal_SetNsel(void);
extern void radio_hal_SpiWriteByte(U8 byteToWrite);
extern void radio_hal_SpiWriteData(U8 byteCount, U8 *pData);
//****************************************************************//
//????: si446x_Custom_write_tx_fifo
//????: ???????????????Tx FIFO
//?    ?:
//? ? ?:
//?    ?:????????,??????TxFIFO
//????: 2015.05.30,Robin????
//***************************************************************//
void si446x_Custom_write_tx_fifo(uint8_t numBytes)
{
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(SI446X_CMD_ID_WRITE_TX_FIFO);
    while (numBytes--)
    {
        radio_hal_SpiWriteByte(0xAA);
    }
    radio_hal_SetNsel();
}
//****************************************************************//
//????: Radio_WriteToFIFO
//????: ???????FIFO
//?    ?:
//? ? ?:
//?    ?:??????Tx FIFO,???????,???????
//????: 2015.05.28,Robin????
//***************************************************************//
void Radio_WriteToFIFO(uint8_t *buf, uint8_t len)
{
    //    uint8_t i, Length;
    uint16_t totalLen = 0;

    //    Length = len;
    fixRadioTxPacket[0] = len;
    memcpy((U8 *)&fixRadioTxPacket[1], buf, len);
    TxType = WakeupFrame;
    RadioPacketLength = len + 1;
    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);

    si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_TX_BIT); //????Fifo
    si446x_get_int_status(0u, 0u, 0u);                      //
    if (TxType)
    {
        NumOfHadTxBytes = 64u;
        // Fifo
        // Length = TxPtr->Length + 1u;
        si446x_Custom_write_tx_fifo(64u);

        totalLen = RadioTxWakeuoPacketLength + len + 1 + 4; //前导码+数据长度+同步码
                                                            // si446x_set_property(0x11,4u,0x01,CustomLongTxSyncWord);
        si446x_set_property(0x11, 4u, 0x01, 0x55, 0x55, 0x55, 0x55);
        si446x_set_property(0x12, 0x02, 0x11, (totalLen - 1) >> 8, totalLen - 1);

        si446x_start_tx(0u, 0x80, 0);
    }
}
/*!
 *  Check if Packet sent IT flag or Packet Received IT is pending.
 *
 *  @return   SI4455_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT / SI4455_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT
 *
 *  @note
 *
 */

U8 bRadio_Check_Tx_RX(void)
{
    static uint8_t Length = 0;
    U8 state = 0;
    if (READ_NIRQ_PIN() == 0)
    {
        /* Read ITs, clear pening ones */
        si446x_get_int_status(0u, 0u, 0u);
        state = Si446xCmd.GET_INT_STATUS.PH_PEND;
        if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CHIP_PEND_CMD_ERROR_PEND_BIT)
        {
            /* State change to */
            si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_SLEEP);

            /* Reset FIFO */
            si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);

            /* State change to */
            si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_RX);
        }

        if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_SENT_PEND_BIT)
        {
            TXSending = 0;

            if (TxType == WakeupFrame)
            {
                // si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_TX_BIT);   //????Fifo
                // si446x_get_int_status(0u,0u,0u);                      //
                // si446x_set_property(0x11,4u,0x01,&StandardTxSyncWord);
                // vRadio_StartRX(0,63);
                // si446x_set_property(RF_SYNC_CONFIG_CUSTOM);
                // vRadio_StartRX(0,0);
                // vRadio_Init();
                TxType = 0;
            }
            // else
            vRadio_StartRX(0, 0);
            // vRadio_Init();
            // return SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_SENT_PEND_BIT;
        }

        if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_RX_PEND_BIT)
        {
            /* Packet RX */

            /* Get payload length */
            si446x_fifo_info(0x00);
            RadioPacketLength = Si446xCmd.FIFO_INFO.RX_FIFO_COUNT;
            si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &fixRadioPacket[0]);
            si446x_frr_a_read(1);/* 读取锁存器值 */
            g_rssi_value = Si446xCmd.FRR_A_READ.FRR_A_VALUE;
            vRadio_StartRX(0, 0);

            // return SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_RX_PEND_BIT;
        }
        if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_PH_STATUS_REP_PH_STATUS_TX_FIFO_ALMOST_EMPTY_BIT)
        {
            if (TxType == WakeupFrame)
            {
                NumOfRemainBytes = RadioTxWakeuoPacketLength - NumOfHadTxBytes;
                if (NumOfRemainBytes > RADIO_TX_ALMOST_EMPTY_THRESHOLD)
                {
                    si446x_Custom_write_tx_fifo(RADIO_TX_ALMOST_EMPTY_THRESHOLD);
                    NumOfHadTxBytes += RADIO_TX_ALMOST_EMPTY_THRESHOLD;
                    Length = RadioPacketLength;
                }
                else if (Length > 44)
                {
                    // Fifo
                    si446x_write_tx_fifo(4u, (U8 *)CustomTxSyncWord);
                    // Fifo
                    si446x_write_tx_fifo(44, (uint8_t *)fixRadioTxPacket);

                    Length = RadioPacketLength - 44;
                }
                else
                {
                    if (RadioPacketLength > 44)
                    {
                        // Fifo
                        si446x_write_tx_fifo(Length, (uint8_t *)&fixRadioTxPacket[44]);
                    }
                    else
                    {
                        Length = RadioPacketLength;
                        si446x_write_tx_fifo(4u, (U8 *)CustomTxSyncWord);
                        // Fifo
                        si446x_write_tx_fifo(Length, (uint8_t *)&fixRadioTxPacket[0]);
                    }
                }
            }
            // return SI446X_CMD_GET_PH_STATUS_REP_PH_STATUS_TX_FIFO_ALMOST_EMPTY_BIT;
        }
        if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_STATUS_CRC_ERROR_BIT)
        {
            /* Reset FIFO */
            si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);
        }
        return state;
    }

    return 0;
}

/*!
 *  Check if Packet sent IT flag or Packet Received IT is pending.
 *
 *  @return   SI4455_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT / SI4455_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT
 *
 *  @note
 *
 */
#if 0
U8 bRadio_Check_Tx_RX(void)
{
//  if(RF_NIRQ == 0)
	if ( READ_NIRQ_PIN() == 0)
  {
      /* Read ITs, clear pening ones */
      si446x_get_int_status(0u, 0u, 0u);

	  if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CHIP_PEND_CMD_ERROR_PEND_BIT)
      {
      	/* State change to */
      	si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_SLEEP);
	
	  	/* Reset FIFO */
      	si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);
      
	  	/* State change to */
        si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_RX);
      }

      if(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_SENT_PEND_BIT)
      {
				TXSending=0;
        return SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_SENT_PEND_BIT;
      }

      if(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_RX_PEND_BIT)
      {
        /* Packet RX */

        /* Get payload length */
        si446x_fifo_info(0x00);
				RadioPacketLength = Si446xCmd.FIFO_INFO.RX_FIFO_COUNT;
        si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &fixRadioPacket[0]);
        vRadio_StartRX(0,0);

        return SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_RX_PEND_BIT;
      }
      
	  if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PH_STATUS_CRC_ERROR_BIT)
      {
      	/* Reset FIFO */
      	si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);
      }

  }

  return 0;
}
#endif
/*!
 *  Set Radio to RX mode. .
 *
 *  @param channel Freq. Channel,  packetLength : 0 Packet handler fields are used , nonzero: only Field1 is used
 *
 *  @note
 *
 */
void vRadio_StartRX(U8 channel, U8 packetLenght)
{
    //   si446x_set_property(0x11,4u,0x01,0xCB,0x89,0xCB,0x89);
    //	//si446x_set_property(RF_SYNC_CONFIG_CUSTOM);
	/*设置前导码*/
    si446x_set_property(0x12, 0x02, 0x11, 0x00, 63);
    // Read ITs, clear pending ones
    si446x_get_int_status(0u, 0u, 0u);

    // Reset the Rx Fifo
    si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_RX_BIT);

    /* Start Receiving packet, channel 0, START immediately, Packet length used or not according to packetLength */
    si446x_start_rx(channel, 0u, packetLenght,
                    SI446X_CMD_START_RX_ARG_NEXT_STATE1_RXTIMEOUT_STATE_ENUM_NOCHANGE,
                    SI446X_CMD_START_RX_ARG_NEXT_STATE2_RXVALID_STATE_ENUM_READY,
                    SI446X_CMD_START_RX_ARG_NEXT_STATE3_RXINVALID_STATE_ENUM_RX);
}

/*!
 *  Set Radio to TX mode, variable packet length.
 *
 *  @param channel Freq. Channel, Packet to be sent length of of the packet sent to TXFIFO
 *
 *  @note
 *
 */
void vRadio_StartTx_Variable_Packet(U8 channel, U8 *pioRadioPacket, U8 length)
{
    /* Leave RX state */
    si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);

    /* Read ITs, clear pending ones */
    si446x_get_int_status(0u, 0u, 0u);

    /* Reset the Tx Fifo */
    si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_FIFO_TX_BIT);

    // si446x_set_property(0x11,4u,0x01,&StandardTxSyncWord);
    // si446x_set_property(RF_SYNC_CONFIG_CUSTOM);
    si446x_set_property(0x12, 0x02, 0x11, 0x00, length - 1);
    /* Fill the TX fifo with datas */
    si446x_write_tx_fifo(length, pioRadioPacket);

    /* Start sending packet, channel 0, START immediately */
    si446x_start_tx(channel, 0x80, 0);
}
U8 RF_DATA_GET(U8 *Buf)
{
    U8 len = fixRadioPacket[0];
    if (fixRadioPacket[0] >= 0x10)
    {
        memcpy(Buf, &fixRadioPacket[1], fixRadioPacket[0]);
        fixRadioPacket[0] = 0;
        return len;
    }
    return 0;
}
U8 si4460_RF_RX(U8 *Buf)
{

    if (SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_RX_PEND_BIT & bRadio_Check_Tx_RX())
    {
        if (fixRadioPacket[0] <= 0x40)
        {
			memcpy(Buf, &fixRadioPacket[1], fixRadioPacket[0]);
            return fixRadioPacket[0];
        }
    }
    return 0;
}

void si4460_RF_TX(U8 *Buf, U8 len)
{
    U8 bufT[50];
    bufT[0] = len;
    memcpy((U8 *)&bufT[1], Buf, len);
    vRadio_StartTx_Variable_Packet(0, bufT, len + 1);
}

U8 RF_TX_Finish(void)
{
    if (SI446X_CMD_GET_INT_STATUS_REP_PH_PEND_PACKET_SENT_PEND_BIT & bRadio_Check_Tx_RX())
    {
        return 1;
    }
    return 0;
}

void RF_Sleep()
{
    si446x_change_state(SI446X_CMD_START_RX_ARG_NEXT_STATE1_RXTIMEOUT_STATE_ENUM_SLEEP);
}

void si4460_InitRF(float frequence)
{
    vRadio_Init_with_fre(frequence);
    vRadio_StartRX(0, 0);
}
uint8_t si4460_rssi_read(void)
{
        uint8_t rssi_value = g_rssi_value;
        g_rssi_value = 0;
        return rssi_value;
}
