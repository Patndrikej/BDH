//******************************************************************************
#include <stddef.h>
#include "stm32l1xx.h"
#include <header.h>

int main(void) {
	int i = 0;
	int l = 0;

	initialization();	//inicializacia premennych a usart, spi1, gpioc

	mySPI_SendData(0x20, 0x67); //LIS3DH nastavenie akcelerometra na citanie hodnot

	get_Y_data();

	y_max = y_min = y_old = y;	//1x sa vsetko nastavi na rovnaku hodnotu

	while (i <= 9) {	//cyklus 0-9 podla pola values
		get_Y_data();

		if (y > y_max) {
			y_max = y;
		}

		if (y < y_min) {
			y_min = y;
		}

		values[i] = y;	//ukladanie Y do pola[i]
		y_old = y;		//uchovanie predchadzajucej hodnoty ak by nasledujuca bola zla

		i++;
	}

	get_Y_average();

	if (y_min == 0) {
		y_min = avg;
	}
	if (y_max == 255) {
		y_max = avg;
	}

	threshold = get_threshold();
	i = 0;

	while (1) {
		reset_kroky();

		get_Y_data();
		i = shift_values(i);
		get_Y_average();

		if (l == 29) {
			val_maxmin[l] = avg;	//pridanie poslednej hodnoty (29) do pola

			update_min_max();

			threshold = get_threshold();
			l = 0;
		} else {
			val_maxmin[l] = avg;
			l++;
		}

		if (avg < threshold && status == 1 && (avg > threshold+3 || avg < threshold-3)) {
			kroky++;
			status = 0;
		} else if (avg > threshold && status == 0) {
			status = 1;
		}

		sprintf(send, "%d      %d      %d      %d      %i\r\n", y_min, y_max, avg, threshold, kroky);
		USARTp_start(send);

		delay(5000);

		y_old = y;
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
