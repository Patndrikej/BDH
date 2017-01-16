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
char send[8];
int j;

int y;
int y_min;
int y_max;
int y_old;
int values[10];			//pole na priemerovanie hodnot Y z akcelerometra
int val_maxmin[30];		//pole z ktoreho sa urci max a min hodnoty
int kroky;
double dlzka_kroku;

uint16_t y_t;			//hodnota Y z akcelerometra
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
void shift_values();
int get_Y_average();
int get_threshold();
int vypocitaj_vzdialenost();

#endif /* HEADER_H_ */
