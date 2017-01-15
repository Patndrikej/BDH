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
char Rec;
char send[64];
int j;

int y;
int y_min;
int y_max;
int y_old;
int values[10];
int val_maxmin[30];
int status;
int kroky;
int avg;
int threshold;

uint16_t y_t;
uint8_t buttonState;

void initialization();
void init_button();
void usart_init();
void init_SPI1(void);

void Send_data(char send[]);
void USARTp_start(char send[]);
uint16_t getSPIdata(uint8_t adress);
void mySPI_SendData(uint8_t adress, uint8_t data);

void reset_kroky();
void delay(int c);
void update_min_max();
void get_Y_data();
int shift_values(int x);
void get_Y_average();
int get_threshold();

#endif /* HEADER_H_ */
