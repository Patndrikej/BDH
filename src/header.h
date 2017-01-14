/*
 * header.h
 *
 *  Created on: 13. 1. 2017
 *      Author: Patrik
 */

#ifndef HEADER_H_
#define HEADER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "stm32l1xx.h"

int value;
char send[8];
int j;

void usart_init();
void USART2_IRQHandler(void);
void Send_data(char send[]);
void USARTp_start(char send[]);
uint16_t get_adc_value1();
uint16_t get_adc_value2();
uint16_t get_adc_value3();

void init_SPI1(void);
unsigned char readWriteSPI1(unsigned char txData);
uint16_t tahajSPIdata(uint8_t adress);
void mySPI_SendData(uint8_t adress, uint8_t data);

//void delay(unsigned int mseconds);
//void set_max();
//void set_min();
//void set_old();
//void filter();
//void initial_filter();
//void set_threshold();

#endif /* HEADER_H_ */
