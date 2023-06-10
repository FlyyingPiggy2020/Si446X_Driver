#ifndef _SPI_H_
#define _SPI_H_

#include "includes.h"

void Si4460_PWRDN_Out(uint8_t isHigh);
void Si4460_SCS_Out(uint8_t isHigh);
uint8_t Si4460_NIRQ_State(void);

#define FSK  	0
#define OOKRX   1
#define OOKTX   2

#define RCC_Si4460_PWRDN()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define Si4460_PWRDN_PORT       GPIOA
#define Si4460_PWRDN_PIN        GPIO_PIN_15

//SCS/NSEL
#define RCC_Si4460_NSEL()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define Si4460_NSEL_PORT        GPIOB
#define Si4460_NSEL_PIN         GPIO_PIN_3

//SCK/SCLK
#define RCC_Si4460_CLK()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define Si4460_CLK_PORT         GPIOB
#define Si4460_CLK_PIN          GPIO_PIN_6

//GPIO1/MOSI
#define RCC_Si4460_SDIO()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define Si4460_SDIO_PORT        GPIOB
#define Si4460_SDIO_PIN         GPIO_PIN_4

//SDOI/MISO
#define RCC_Si4460_SDOI()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define Si4460_SDOI_PORT        GPIOB
#define Si4460_SDOI_PIN         GPIO_PIN_5

//GPIO2/NIRQ
#define RCC_Si4460_NIRQ()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define Si4460_NIRQ_PORT        GPIOB
#define Si4460_NIRQ_PIN         GPIO_PIN_7


//读输入引脚
#define READ_SDOI_PIN()				Si4460_SDOI_State()					//PIN 2
#define READ_NIRQ_PIN()				Si4460_NIRQ_State()

//写输出引脚
#define Set_GPIO_SCS()				Si4460_SCS_Out(1)
#define Clr_GPIO_SCS()				Si4460_SCS_Out(0)

#define Set_GPIO_SCK()				Si4460_SCK_Out(1)
#define Clr_GPIO_SCK()				Si4460_SCK_Out(0)

#define Set_GPIO_SDIO()				Si4460_SDIO_Out(1)				//PIN 8
#define Clr_GPIO_SDIO()				Si4460_SDIO_Out(0)


#define Set_GPIO_PWRDN()			Si4460_PWRDN_Out(1)
#define Clr_GPIO_PWRDN()			Si4460_PWRDN_Out(0)

//---------------------------------------------
#define SDO_GPIO_IN()              Si4460_SDO_InOut(1)
#define SDO_GPIO_OUT()             Si4460_SDO_InOut(0)

#define SDI_GPIO_IN()				Si4460_SDI_InOut(1)
#define SDI_GPIO_OUT()				Si4460_SDI_InOut(0)

#define SCS_GPIO_IN()               Si4460_SCS_InOut(1)		//6脚
#define SCS_GPIO_OUT()              Si4460_SCS_InOut(0)

#define SCK_GPIO_IN()               Si4460_SCK_InOut(1)
#define SCK_GPIO_OUT()              Si4460_SCK_InOut(0)

#define GOI2_GPIO_IN()						Si4460_GPIO2_InOut(1)
#define GOI2_GPIO_OUT()						Si4460_GPIO2_InOut(0)

#define NIRQ_GPIO_IN()						Si4460_NIRQ_InOut(1)
#define NIRQ_GPIO_OUT()						Si4460_NIRQ_InOut(0)

#define PWRDN_GPIO_IN()						Si4460_PWRDN_InOut(1)
#define PWRDN_GPIO_OUT()					Si4460_PWRDN_InOut(0)



void vSpi_WriteDataSpi1(U8 biDataInLength, U8 *pabiDataIn);//void vSpi_WriteDataSpi1(U8 biDataInLength, U8 *pabiDataIn)
void vSpi_ReadDataSpi1(U8 biDataOutLength, U8 *paboDataOut);//  U8 bSpi_ReadWriteSpi1(U8 biDataIn) 
U8 bSpi_ReadWriteSpi1(U8 biDataIn) ;
void InitSi446xSPI(void);
uint8_t Si4460_SDOI_State(void);
#endif

