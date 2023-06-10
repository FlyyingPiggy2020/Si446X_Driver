#define SI4463

#ifdef SI4463


#include "main.h"
#include "spi.h"
static GPIO_InitTypeDef GPIO_InitStructure;



/* USER CODE BEGIN 2 */
uint8_t Si4460_SDOI_State(void)
{
	if(HAL_GPIO_ReadPin(Si4460_SDOI_PORT,Si4460_SDOI_PIN)== 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}	
}
uint8_t Si4460_NIRQ_State(void)
{
	if(HAL_GPIO_ReadPin(Si4460_NIRQ_PORT,Si4460_NIRQ_PIN)== 1)
		return 1;
	else
		return 0;
}

void Si4460_SCS_Out(uint8_t isHigh)
{
	if(isHigh)
		HAL_GPIO_WritePin(Si4460_NSEL_PORT,Si4460_NSEL_PIN,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(Si4460_NSEL_PORT,Si4460_NSEL_PIN,GPIO_PIN_RESET);
}
void Si4460_SCK_Out(uint8_t isHigh)
{
   if(isHigh)
		HAL_GPIO_WritePin(Si4460_CLK_PORT,Si4460_CLK_PIN,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(Si4460_CLK_PORT,Si4460_CLK_PIN,GPIO_PIN_RESET);
}
void Si4460_SDIO_Out(uint8_t isHigh)
{
   	if(isHigh)
		HAL_GPIO_WritePin(Si4460_SDIO_PORT,Si4460_SDIO_PIN,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(Si4460_SDIO_PORT,Si4460_SDIO_PIN,GPIO_PIN_RESET);
}
void Si4460_PWRDN_Out(uint8_t isHigh)
{
   if(isHigh)
		HAL_GPIO_WritePin(Si4460_PWRDN_PORT,Si4460_PWRDN_PIN,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(Si4460_PWRDN_PORT,Si4460_PWRDN_PIN,GPIO_PIN_RESET);
}


//==============================================================================
void Si4460_SDO_InOut(uint8_t isIn)
{
   // GPIO_InitTypeDef GPIO_InitStruct;
	RCC_Si4460_SDIO();
	/*Configure GPIO pin : PtPin */
	GPIO_InitStructure.Pin = Si4460_SDIO_PIN;
	if(isIn)
	{
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLUP;  		
	}
		
	else
	{
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	}
  	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;  
  	HAL_GPIO_Init(Si4460_SDIO_PORT, &GPIO_InitStructure); 
}
void Si4460_SDI_InOut(uint8_t isIn)
{
   // GPIO_InitTypeDef GPIO_InitStruct;
	RCC_Si4460_SDOI();
	/*Configure GPIO pin : PtPin */
	GPIO_InitStructure.Pin = Si4460_SDOI_PIN;
	if(isIn)
	{
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLUP;  		
	}		
	else
	{
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	}

  	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;  
  	HAL_GPIO_Init(Si4460_SDOI_PORT, &GPIO_InitStructure); 
}
void Si4460_SCS_InOut(uint8_t isIn)
{
   // GPIO_InitTypeDef GPIO_InitStruct;
	RCC_Si4460_NSEL();
	/*Configure GPIO pin : PtPin */
	GPIO_InitStructure.Pin = Si4460_NSEL_PIN;

	if(isIn)
	{
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLUP;  		
	}		
	else
	{
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	}

  	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;  
	HAL_GPIO_Init(Si4460_NSEL_PORT, &GPIO_InitStructure); 
}
void Si4460_SCK_InOut(uint8_t isIn)
{
   // GPIO_InitTypeDef GPIO_InitStruct;
	RCC_Si4460_CLK();
	/*Configure GPIO pin : PtPin */
	GPIO_InitStructure.Pin = Si4460_CLK_PIN;

	if(isIn)
	{
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLUP;  		
	}		
	else
	{
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	}
  	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(Si4460_CLK_PORT, &GPIO_InitStructure); 
}
void Si4460_NIRQ_InOut(uint8_t isIn)
{
   // GPIO_InitTypeDef GPIO_InitStruct;
	RCC_Si4460_NIRQ(); 
	/*Configure GPIO pin : PtPin */
	GPIO_InitStructure.Pin = Si4460_NIRQ_PIN;

	if(isIn)
	{
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLUP;  		
	}		
	else
	{
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	}
  	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;  
	HAL_GPIO_Init(Si4460_NIRQ_PORT, &GPIO_InitStructure); 
}
void Si4460_PWRDN_InOut(uint8_t isIn)
{
       // GPIO_InitTypeDef GPIO_InitStruct;
	RCC_Si4460_PWRDN();
	/*Configure GPIO pin : PtPin */
	GPIO_InitStructure.Pin = Si4460_PWRDN_PIN;

	if(isIn)
	{
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		GPIO_InitStructure.Pull = GPIO_PULLUP;  		
	}		
	else
	{
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	}
  	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;  
	HAL_GPIO_Init(Si4460_PWRDN_PORT, &GPIO_InitStructure); 
}
//void Si4460_GPIO2_InOut(uint8_t isIn)
//{
//       // GPIO_InitTypeDef GPIO_InitStruct;
//	RCC_APB2PeriphClockCmd(RCC_Si4460_GPIO2, ENABLE); 
//	/*Configure GPIO pin : PtPin */
//	GPIO_InitStructure.GPIO_Pin = Si4460_GPIO2_PIN;

//	if(isIn)
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	else
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
//  GPIO_Init(Si4460_GPIO2_PORT, &GPIO_InitStructure); 
//}

U8 bSpi_ReadWriteSpi1(U8 biDataIn)  //bSpi_ReadWriteSpi1(byteToWrite);
{  

    U8 i = 0;
    U8 bValue = 0;
	Clr_GPIO_SCS();	
	Clr_GPIO_SCK();
	for(i=0; i<8; i++)
	{
		if(biDataIn & 0x80)  //write data
		{
			Set_GPIO_SDIO();
		}
		else
		{ 
		    Clr_GPIO_SDIO();
		}
		biDataIn <<= 1;		
		bValue <<= 1;		
//		Set_GPIO_SCK();
		if(READ_SDOI_PIN())
		{
		    bValue++;    //read data
		}
		Set_GPIO_SCK();
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		Clr_GPIO_SCK();             
	}
  // Restore interrupts after SPI transfer
  //EA = gRestoreEA;//enable all interrupt
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
		__asm("NOP");
  return bValue;
}
/*!
 * This function is used to send data over SPI1 no response expected.
 *
 *  @param[in] biDataInLength  The length of the data.
 *  @param[in] *pabiDataIn     Pointer to the first element of the data.
 *
 *  @return None
 */
void vSpi_WriteDataSpi1(U8 biDataInLength, U8 *pabiDataIn)
{
  while (biDataInLength--) {
    bSpi_ReadWriteSpi1(*pabiDataIn++);
  }
}

/*!
 * This function is used to read data from SPI1.
 *
 *  \param[in] biDataOutLength  The length of the data.
 *  \param[out] *paboDataOut    Pointer to the first element of the response.
 *
 *  \return None
 */
void vSpi_ReadDataSpi1(U8 biDataOutLength, U8 *paboDataOut)
{
  // send command and get response from the radio IC
  while (biDataOutLength--) {
    *paboDataOut++ = bSpi_ReadWriteSpi1(0xFF);
  }
}
void InitSi446xSPI(void)
{
	NIRQ_GPIO_IN();			
	 
	PWRDN_GPIO_OUT();			//5½Å
	Clr_GPIO_PWRDN();
	
	SCS_GPIO_OUT();			
	Clr_GPIO_SCS();
	
	SCK_GPIO_OUT();			
	Clr_GPIO_SCK();
	
	SDO_GPIO_OUT();			
	Clr_GPIO_SDIO();
	
	SDI_GPIO_IN();			
}

#endif
