/*
 * sources1.c
 *
 *  Created on: 13. 1. 2017
 *      Author: Patrik
 */

#include <header.h>

void usart_init() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	GPIO_InitTypeDef GPIO_usrt;

	GPIO_usrt.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_usrt.GPIO_Mode = GPIO_Mode_AF;
	GPIO_usrt.GPIO_OType = GPIO_OType_PP;
	GPIO_usrt.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_usrt.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIOA, &GPIO_usrt);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	/* Enable the USARTx Interrupt */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//interrupt
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART2, ENABLE);

}

void Send_data(char send[]) {
	int j = 0;
	while (send[j] != 0) {
		USART_SendData(USART2, send[j]);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			;
		j++;
	}
	USART_SendData(USART2, '\r');
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
		;
}

void USARTp_start(char send[]) {
	if (j == 0) {
		USART_SendData(USART2, send[j]);
		j++;
	}
}

void USART2_IRQHandler(void) {
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		Rec = USART_ReceiveData(USART2);
	} else if (USART_GetITStatus(USART2, USART_IT_TC) != RESET) {
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		if (j > 0) {
			if (send[j] != '\0') {
				USART_SendData(USART2, send[j]);
				j++;
			} else {
				USART_SendData(USART2, '\r');
				j = 0;
			}
		}
	}
}

void init_SPI1(void) {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CRCPolynomial = SPI_CRC_Rx;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
	SPI_Init(SPI1, &SPI_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitTypeDef gpioInitStruc;

	gpioInitStruc.GPIO_Mode = GPIO_Mode_AF;
	gpioInitStruc.GPIO_OType = GPIO_OType_PP;
	gpioInitStruc.GPIO_Speed = GPIO_Speed_40MHz;
	gpioInitStruc.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioInitStruc.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &gpioInitStruc);

	gpioInitStruc.GPIO_OType = GPIO_OType_PP;
	gpioInitStruc.GPIO_Speed = GPIO_Speed_40MHz;
	gpioInitStruc.GPIO_PuPd = GPIO_PuPd_UP;
	gpioInitStruc.GPIO_Pin = GPIO_Pin_6;
	gpioInitStruc.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &gpioInitStruc);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	SPI_Cmd(SPI1, ENABLE);

}

uint16_t getSPIdata(uint8_t adress) {

	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	adress = 0x80 | adress;
	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
		;
	SPI_I2S_SendData(SPI1, adress);
	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
		;
	SPI_I2S_ReceiveData(SPI1); //Clear RXNE bit

	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
		;
	SPI_I2S_SendData(SPI1, 0x00); //Dummy byte to generate clock
	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
		;
	GPIO_SetBits(GPIOB, GPIO_Pin_6);

	return SPI_I2S_ReceiveData(SPI1);
}

void mySPI_SendData(uint8_t adress, uint8_t data) {

	GPIO_ResetBits(GPIOB, GPIO_Pin_6);

	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
		;
	SPI_I2S_SendData(SPI1, adress);
	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
		;
	SPI_I2S_ReceiveData(SPI1);

	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
		;
	SPI_I2S_SendData(SPI1, data);
	while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
		;
	SPI_I2S_ReceiveData(SPI1);

	GPIO_SetBits(GPIOB, GPIO_Pin_6);
}

void init_button() {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	GPIO_InitTypeDef gpioInitStruc;

	gpioInitStruc.GPIO_Mode = GPIO_Mode_IN;
	gpioInitStruc.GPIO_PuPd = GPIO_PuPd_UP;
	gpioInitStruc.GPIO_Pin = GPIO_Pin_13;
	gpioInitStruc.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIOC, &gpioInitStruc);
}

void initialization(){
	kroky = 0;
	buttonState = 1;
	avg = 0;
	threshold = 0;
	status = 0;
	y = 0;
	y_min = 0;
	y_max = 0;
	y_old = 0;
	values[10] = 0;
	val_maxmin[30] = 0;

	init_SPI1();
	usart_init();
	init_button();
}

void reset_kroky(){
	buttonState = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);

	if (buttonState == 0) {
		kroky = 0;
	}
}

void delay(int c){
	for (int m = 0; m < c; m++) {}
}

void update_min_max(){
	y_min = val_maxmin[0];
	y_max = val_maxmin[0];

	for (int k = 1; k <= 29; k++) {
		if (val_maxmin[k] < y_min) {
			y_min = val_maxmin[k];
		}
		if (val_maxmin[k] > y_max) {
			y_max = val_maxmin[k];
		}
	}
}

void get_Y_data(){
	y_t = 0;
	y_t = getSPIdata(0x2B);	//get Y_high, 2B

	y = (int) y_t;	//pretypovanie z uint16_t na int

	if (y > 255) {	//osetrenie Y ak by hodnota bola viac ako 255 tak 0
		y = y_old;
	}
}

int shift_values(int x){
	if (x < 9) {
		values[0] = values[1];
		values[1] = values[2];
		values[2] = values[3];
		values[3] = values[4];
		values[4] = values[5];
		values[5] = values[6];
		values[6] = values[7];
		values[7] = values[8];
		values[8] = values[9];
		values[9] = y;

		x++;
	} else if (x == 9) {
		values[0] = values[1];
		values[1] = values[2];
		values[2] = values[3];
		values[3] = values[4];
		values[4] = values[5];
		values[5] = values[6];
		values[6] = values[7];
		values[7] = values[8];
		values[8] = values[9];
		values[9] = y;
		x = 0;
	}

	return x;
}

void get_Y_average(){
	avg = (values[0] + values[1] + values[2] + values[3] + values[4]
			+ values[5] + values[6] + values[7] + values[8] + values[9])
			/ 10;
}

int get_threshold(){
	return (y_max + y_min) / 2;
}
