/*
 * sources.c
 *
 *  Created on: 13. 1. 2017
 *      Author: Patrik
 */

#include "header.h"

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
	USART_InitStructure.USART_Mode = /*USART_Mode_Rx | */USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
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

uint16_t get_adc_value1(){
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_16Cycles);
	  ADC_SoftwareStartConv(ADC1);
	  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}

	  return ADC_GetConversionValue(ADC1);
}

uint16_t get_adc_value2(){
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_16Cycles);
	  ADC_SoftwareStartConv(ADC1);
	  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}

	  return ADC_GetConversionValue(ADC1);
}

uint16_t get_adc_value3(){
	  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_16Cycles);
	  ADC_SoftwareStartConv(ADC1);
	  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}

	  return ADC_GetConversionValue(ADC1);
}





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


uint16_t tahajSPIdata(uint8_t adress){

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

//void delay(unsigned int mseconds){
//    clock_t goal = mseconds + clock();
//    while (goal > clock());
//}

//void set_max(){
//  	if(x > x_max){
//  		x_max = x;
//  	}
//
//  	if(y > y_max){
//  		y_max = y;
//  	}
//
//  	if(z > z_max){
//  		z_max = z;
//  	}
//}
//
//void set_min(){
//  	if(x < x_min){
//  		x_min = x;
//  	}
//
//  	if(y < y_min){
//  		y_min = y;
//  	}
//
//  	if(z < z_min){
//  		z_min = z;
//  	}
//}
//
//void set_old(){
//	x_old = x;
//	y_old = y;
//	z_old = z;
//}
//
//void filter(){	// 48 - 57 = cisla v ascii
//	if((x >= 48 && x <= 57) && (x < x_old - 10 || x > x_old + 10)){
//		x = x_old;
//	}
//
//	if((y>= 48 && y <= 57) && (y < y_old - 10 || y > y_old + 10)){
//		y = y_old;
//	}
//
//	if((z >= 48 && z <= 57) && (z < z_old - 10 || z > z_old + 10)){
//		z = z_old;
//	}
//}
//
//void initial_filter(){
//	if(x >= 48 && x <= 57){
//		x = x_old;
//	}
//
//	if(y>= 48 && y <= 57){
//		y = y_old;
//	}
//
//	if(z >= 48 && z <= 57){
//		z = z_old;
//	}
//}
//
//void set_threshold() {
//	threshold = (x_max / x_min)/2;
//}
