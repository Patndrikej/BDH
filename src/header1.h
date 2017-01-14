/*
 * header1.h
 *
 *  Created on: 13. 1. 2017
 *      Author: Patrik
 */

#ifndef HEADER1_H_
#define HEADER1_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "stm32l1xx.h"

int value;
char Rec;
char send[8];
int j;

void usart_init();
void Send_data(char send[]);
void USARTp_start(char send[]);
//void USART2_IRQHandler(void);

//uint16_t get_adc_value1();
//uint16_t get_adc_value2();
//uint16_t get_adc_value3();

void init_SPI1(void);
uint16_t getSPIdata(uint8_t adress);
void mySPI_SendData(uint8_t adress, uint8_t data);

#endif /* HEADER1_H_ */
