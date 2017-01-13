/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO/STM32
**                STMicroelectronics STM32Lxx Standard Peripherals Library
**
**  Distribution: The file is distributed "as is", without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"
#include "stm32l1xx_spi.h"
#include "stm32l1xx_rcc.h"
#include <stdio.h>





/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */


/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

char send[8];
int j;

void init_SPI1(void){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_CRCPolynomial = SPI_CRC_Rx;
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft| SPI_NSSInternalSoft_Set;
	SPI_Init(SPI1,&SPI_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpioInitStruc;

	gpioInitStruc.GPIO_Mode = GPIO_Mode_AF;
	gpioInitStruc.GPIO_OType = GPIO_OType_PP;
	gpioInitStruc.GPIO_Speed=GPIO_Speed_40MHz;
	gpioInitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInitStruc.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOA,&gpioInitStruc);

	gpioInitStruc.GPIO_OType = GPIO_OType_PP;
	gpioInitStruc.GPIO_Speed = GPIO_Speed_40MHz;
	gpioInitStruc.GPIO_PuPd = GPIO_PuPd_UP;
	gpioInitStruc.GPIO_Pin = GPIO_Pin_6;
	gpioInitStruc.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &gpioInitStruc);



	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);

	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	SPI_Cmd(SPI1, ENABLE);

}

unsigned char readWriteSPI1(unsigned char txData)
{
	unsigned char  rxData;

	SPI1->DR = txData;
	// wait until TXE = 1
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET);
	// wait until RXNE = 1
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET);
	// read the rx buff to clear the RXNE flag (garbage)
	rxData = SPI1->DR;

	return rxData;
}


uint8_t tahajSPIdata(uint8_t adress){

	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	adress = 0x80| adress;
	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, adress);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1); //Clear RXNE bit

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, 0x00); //Dummy byte to generate clock
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	GPIO_SetBits(GPIOB,GPIO_Pin_6);

	return SPI_I2S_ReceiveData(SPI1);
}

void mySPI_SendData(uint8_t adress, uint8_t data){

	GPIO_ResetBits(GPIOB, GPIO_Pin_6);

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, adress);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);

	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, data);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI1);

	GPIO_SetBits(GPIOB, GPIO_Pin_6);
}

void init_USART(){
	GPIO_InitTypeDef GPIO_usrt;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	//GPIO pre RX a TX
	GPIO_usrt.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_usrt.GPIO_Mode = GPIO_Mode_AF;
	GPIO_usrt.GPIO_OType = GPIO_OType_PP;
	GPIO_usrt.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_usrt.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIOA,&GPIO_usrt);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	//nastavenie USART
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	//nastavenie preruseni pre USART
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);

}
void posli(char *res){
	int index =0;
	while(res[index] != '\0')
	{
		USART_SendData(USART2, res[index]);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		index++;
	}
	USART_SendData(USART2,' ');
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}
//-------------------------------------------
void usart_init() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//NOPULL or UP
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600*2; //115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//interrupt
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);
}

void USARTp_start(char send[]){
	if (j == 0){
		USART_SendData(USART2,send[j]);
		j++;
	}
}

void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET){
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		if (j > 0){
			if(send[j] != '\0'){
				USART_SendData(USART2,send[j]);
				j++;
			}else{
				USART_SendData(USART2,'\r');
				j = 0;
			}
		}
	}
}
//--------------------------------------------

int main(void)
{

  int i=0;

   int8_t x=0,y=0,z=0;
   int8_t arrayX[10]={0};
   char pole[30]={0};

  /**
  *  IMPORTANT NOTE!
  *  See the <system_*.c> file and how/if the SystemInit() function updates
  *  SCB->VTOR register. Sometimes the symbol VECT_TAB_SRAM needs to be defined
  *  when building the project if code has been located to RAM and interrupts
  *  are used. Otherwise the interrupt table located in flash will be used.
  *  E.g.  SCB->VTOR = 0x20000000;
  */

  /**
  *  At this stage the microcontroller clock setting is already configured,
  *  this is done through SystemInit() function which is called from startup
  *  file (startup_stm32l1xx_hd.s) before to branch to application main.
  *  To reconfigure the default setting of SystemInit() function, refer to
  *  system_stm32l1xx.c file
  */

  /* TODO - Add your application code here */

  init_SPI1();
  usart_init();

  mySPI_SendData(0x20,0x67); //LIS3DH nastavenie akcelerometra na citanie
  //mySPI_SendData(0x20,0x60);	//LIS2DS12

 //init_CSPin();

  /* Infinite loop */
  while (1)
  {
	 x=tahajSPIdata(0x29);	//X_high
	 y=tahajSPIdata(0x2B);	//Y_high
	 z=tahajSPIdata(0x2D);	//Z_high


	 i++;

	 sprintf(send,"X:%d, Y:%d, Z:%d\r\n", x,y,z );
	 USARTp_start(send);
	  i++;
	 //  posli(pole);
	/* if(i==10){
		 sprintf(pole,"%d",3);
		 posli(pole);
	 }*/
  }
  return 0;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
