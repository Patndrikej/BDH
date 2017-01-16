//******************************************************************************
#include <stddef.h>
#include "stm32l1xx.h"
#include <header.h>

int main(void) {
	int i = 1;
	int l = 0;

	int y_avg = 0;
	int threshold = 0;
	int status = 0;

	initialization();			//inicializacia premennych a usart, spi1, gpioc

	mySPI_SendData(0x20, 0x67); //LIS3DH nastavenie akcelerometra na citanie hodnot

	get_Y_data();				//citanie hodnoty z osi Y

	values[0] = y;
	y_max = y_min = y_old = y;	//1x sa vsetko nastavi na rovnaku hodnotu

	while (i <= 9) {		//cyklus 0-9 podla pola values
		get_Y_data();		//citanie hodnoty z osi Y

		if (y > y_max) {	//ak je aktualne Y vacsie ako Ymax tak je nove max
			y_max = y;
		}

		if (y < y_min) {	//tak isto ako max len mensie
			y_min = y;
		}

		values[i] = y;		//ukladanie Y do pola[i]
		y_old = y;			//ukladanie predchadzajucej hodnoty ak by nasledujuca bola zla

		i++;
	}

	y_avg = get_Y_average();	//priemer z pola values

	if (y_min == 0) {	//ak by hodnota y_min bola 0 tak zle, prirad y_avg
		y_min = y_avg;
	}
	if (y_max == 255) {	//ak by hodnota y_min bola 255 tak zle, prirad y_avg
		y_max = y_avg;
	}

	threshold = get_threshold();	//vypocita threshold na zaklade (y_max+y_min)/2
	i = 0;

//****************MAIN LOOP**********************//
	while (1) {
		reset_kroky();				//ak je stlacene tlacidlo na stm-ku, tak kroky = 0

		get_Y_data();				//citanie hodnoty z osi Y

//*********PRIEMEROVANIE HODNOT Y*****/
		shift_values();				//posunie kazdu hodnotu vo values na poziciu n-1
		y_avg = get_Y_average();	//priemer z pola values
//*********PRIEMEROVANIE HODNOT Y*****/

/***********MAX MIN THRESHOLD**********/
		if (l == 29) {				//ak plati podmienka tak je posledny prvok pola
			val_maxmin[l] = y_avg;	//pridanie poslednej hodnoty (29) do pola

			update_min_max();		//najde nove max a min pre Y

			threshold = get_threshold();	//vypocita threshold na zaklade (y_max+y_min)/2
			l = 0;							//vynuluje iterator
		} else {							//ak nie je posledny prvok
			val_maxmin[l] = y_avg;			//tak sa priradi hodnota y_avg (Y) do pola
			l++;							//zvysi sa iterator
		}
/***********MAX MIN THRESHOLD**********/

/*********PODMIENKA NA KROKY++*********/
		if (y_avg < threshold && status == 1 &&			//podmienka plati ak: hodnota threshold > zpriemerovana Y hodnota cize sa dostane pod threshold
		   (y_avg > threshold+3 || y_avg < threshold-3)) {	//a status je 1 cize hodnota y_avg bola nad threshold-om cize medzi max a thresholdom
														//a osetrenie o +-3 proti sumu
			kroky++;
			status = 0;									//status sa nastavi na 0 t.j. hodnota presla pod threshold medzi min a threshold
		} else if (y_avg > threshold && status == 0) {
			status = 1;									//status sa nastavi na 1 t.j. hodnota presla nad threshold medzi threshold a max
		}
/*********PODMIENKA NA KROKY++*********/

		//sprintf(send, "%d      %d      %d      %d      %d\r\n", y_min, y_max, y_avg, threshold, kroky);
		//sprintf(send, ";%d:%d:%d:%d:%d;", y_min, y_max, y_avg, threshold, kroky);

		sprintf(send, "%d", kroky);
		USARTp_start(send);

		delay(5000);

		y_old = y;		//ukladanie predchadzajucej hodnoty ak by nasledujuca bola zla
	}
//***********************************************//
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
