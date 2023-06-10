#include "includes.h"
#include "spi.h"

/*!
 * File:
 *  radio_hal.c
 *
 * Description:
 *  This file contains RADIO HAL.
 *
 * Silicon Laboratories Confidential
 * Copyright 2011 Silicon Laboratories, Inc.
 */

                /* ======================================= *
                 *              I N C L U D E              *
                 * ======================================= */
#include "includes.h"
#include "spi.h"
                /* ======================================= *
                 *          D E F I N I T I O N S          *
                 * ======================================= */

                /* ======================================= *
                 *     G L O B A L   V A R I A B L E S     *
                 * ======================================= */

                /* ======================================= *
                 *      L O C A L   F U N C T I O N S      *
                 * ======================================= */

                /* ======================================= *
                 *     P U B L I C   F U N C T I O N S     *
                 * ======================================= */
//这几个引脚需要分配MCU的引脚，为了解决编译报错，这里用变量来变通处理
//请务必分配MCU的引脚 RF_PWRDN, RF_NSEL, RF_NIRQ

// SDN = 1;
void radio_hal_AssertShutdown(void)
{
//	RF_SDN = 1;
	Si4460_PWRDN_Out(1);
}

// SDN = 0;
void radio_hal_DeassertShutdown(void)
{
//	RF_SDN = 0;
	Si4460_PWRDN_Out(0);
}

// RF_NSEL = 0;
void radio_hal_ClearNsel(void)
{
 //   RF_NSEL = 0;
	 Si4460_SCS_Out(0);
}

// RF_NSEL = 1;
void radio_hal_SetNsel(void)
{
//    RF_NSEL = 1;
	Si4460_SCS_Out(1);
}

// RF_NIRQ;
U8 radio_hal_NirqLevel(void)
{
U8 ucVol=Si4460_NIRQ_State();	
//    return RF_NIRQ;
	  return ucVol;
}

void radio_hal_SpiWriteByte(U8 byteToWrite)
{
  bSpi_ReadWriteSpi1(byteToWrite);

}

U8 radio_hal_SpiReadByte(void)
{
  return bSpi_ReadWriteSpi1(0xFF);
}

void radio_hal_SpiWriteData(U8 byteCount, U8* pData)
{
  vSpi_WriteDataSpi1(byteCount, pData);
}

void radio_hal_SpiReadData(U8 byteCount, U8* pData)
{
  vSpi_ReadDataSpi1(byteCount, pData);

}

