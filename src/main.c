//******************************************************************************

#include <stddef.h>
#include "stm32l1xx.h"
#include <header1.h>

int main(void) {
	int i = 0;
	uint16_t y_t;
	int y = 0, y_old = 0;
	int avg = 0;
	int y_max, y_min;
	int values[10];
	int val_maxmin[30];
	int threshold;
	int l = 0;
	int status = 0;
	int kroky = 0;

	uint8_t buttonState = 1;

	init_SPI1();
	usart_init();
	init_button();

	y = y_min = y_max = y_old = 0;

	mySPI_SendData(0x20, 0x67); //LIS3DH nastavenie akcelerometra na citanie hodnot

	y_t = getSPIdata(0x2B);	//Y_high, 2B
	y = (int) y_t;
	if (y > 255) {
		y = 0;
	}

	y_min = y_max = y_old = y;

	while (i <= 9) {
		y_t = 0;
		y_t = getSPIdata(0x2B);	//Y_high, 2B
		y = (int) y_t;

		if (y > 255) {
			y = y_old;
		}

		if (y > y_max) {
			y_max = y;
		}

		if (y < y_min) {
			y_min = y;
		}

		values[i] = y;
		y_old = y;

		i++;
	}

	avg = (values[0] + values[1] + values[2] + values[3] + values[4] + values[5]
			+ values[6] + values[7] + values[8] + values[9]) / 10;

	if (y_min == 0) {
		y_min = avg;
	}
	if (y_max == 255) {
		y_max = avg;
	}

	threshold = (y_max + y_min) / 2;
	i = 0;

	while (1) {
		buttonState = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);

		if (buttonState == 0) {
			kroky = 0;
		}

		y_t = 0;
		y_t = getSPIdata(0x2B);	//Y_high, 2B
		y = (int) y_t;

		if (y > 255) {
			y = y_old;
		}

		if (i < 9) {
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

			i++;
		} else if (i == 9) {
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
			i = 0;
		}

		avg = (values[0] + values[1] + values[2] + values[3] + values[4]
				+ values[5] + values[6] + values[7] + values[8] + values[9])
				/ 10;

		if (l == 29) {
			val_maxmin[l] = avg;
			y_min = y_max = val_maxmin[0];

			for (int k = 1; k <= 29; k++) {
				if (val_maxmin[k] < y_min) {
					y_min = val_maxmin[k];
				}
				if (val_maxmin[k] > y_max) {
					y_max = val_maxmin[k];
				}
			}

			threshold = (y_max + y_min) / 2;
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

		sprintf(send, "%d      %d      %d      %d      %d\r\n", y_min, y_max, avg, threshold, kroky);
		USARTp_start(send);

		for (int k = 0; k < 5000; k++) {
		}

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
void __assert_func(const char *file, int line, const char *func,
		const char *failedexpr) {
	while (1) {
	}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr) {
	__assert_func(file, line, NULL, failedexpr);
}
