//******************************************************************************

#include <stddef.h>
#include "stm32l1xx.h"
#include <header1.h>

int main(void) {
	int i = 0;
	uint16_t x_t, y_t, z_t;
	int x = 0, y = 0, z = 0;
	int x_old = 0, y_old = 0, z_old = 0;
	int avg = 0;
	int x_max, y_max, z_max;
	int x_min, y_min, z_min;
	int values[10];
	int val_maxmin[30];
	int threshold;
	int l = 0;
	int status = 0;
	int kroky = 0;
	int avg_min = 0, avg_max = 0;
	int avg_min_count = 0, avg_max_count = 0;

	init_SPI1();
	usart_init();

	x = x_min = x_max = x_old = 0;
	y = y_min = y_max = y_old = 0;

	mySPI_SendData(0x20, 0x67); //LIS3DH nastavenie akcelerometra na citanie hodnot

	//x_t = getSPIdata(0x29);	//X_high 29
	y_t = getSPIdata(0x2B);	//Y_high, 2B
	//z_t = getSPIdata(0x2D);	//Z_high 2D

	//x = (int) x_t;
	y = (int) y_t;
	//z = (int) z_t;

//	if (x > 255) {
//		x = 0;
//	}
	if (y > 255) {
		y = 0;
	}
//	if (z > 255) {
//		z = 0;
//	}

	y_min = y_max = y_old = y;

	while (i <= 9) {
		x_t = y_t = z_t = 0;

		//x_t = getSPIdata(0x29);	//X_high 29
		y_t = getSPIdata(0x2B);	//Y_high, 2B
		//z_t = getSPIdata(0x2D);	//Z_high 2D

		//x = (int) x_t;
		y = (int) y_t;
		//z = (int) z_t;

//		if (x > 255) {
//			x = x_old;
//		}
		if (y > 255) {
			y = y_old;
		}
//		if (z > 255) {
//			z = z_old;
//		}

		if(y > y_max) {
			y_max = y;
		}

		if(y < y_min) {
			y_min = y;
		}

		values[i] = y;

		//x_old = x;
		y_old = y;
		//z_old = z;

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

//	for (int k = 0; k <= 9; k++) {
//		if (values[k] >= avg) {
//			avg_max = avg_max + values[k];
//			avg_max_count++;
//		}
//		if (values[k] <= avg) {
//			avg_min = avg_min + values[k];
//			avg_min_count++;
//		}
//	}
//
//	y_max = avg_max / avg_max_count;
//	y_min = avg_min / avg_min_count;

	threshold = (y_max + y_min) / 2;
	i = 0;

	while (1) {
		x_t = y_t = z_t = 0;

		//x_t = getSPIdata(0x29);	//X_high 29
		y_t = getSPIdata(0x2B);	//Y_high, 2B
		//z_t = getSPIdata(0x2D);	//Z_high 2D

		//x = (int) x_t;
		y = (int) y_t;
		//z = (int) z_t;

//		if (x > 255) {
//			x = x_old;
//		}
		if (y > 255) {
			y = y_old;
		}
//		if (z > 255) {
//			z = z_old;
//		}

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

//		for (int k = 0; k <= 9; k++) {
//			if (values[k] >= avg) {
//				avg_max = avg_max + values[k];
//				avg_max_count++;
//			}
//			if (values[k] <= avg) {
//				avg_min = avg_min + values[k];
//				avg_min_count++;
//			}
//		}
//
//		y_max = avg_max / avg_max_count;
//		y_min = avg_min / avg_min_count;
//		threshold = (y_max + y_min) / 2;

		if(l == 29){
			val_maxmin[l] = avg;
			y_min = y_max = val_maxmin[0];

			for(int k=1;k<=29;k++){
				if(val_maxmin[k] < y_min){
					y_min = val_maxmin[k];
				}
				if(val_maxmin[k] > y_max){
					y_max = val_maxmin[k];
				}
			}

			threshold = (y_max + y_min) / 2;
			l = 0;
		}else{
			val_maxmin[l] = avg;
			l++;
		}

//		if(y_min < 5){
//			y_min = avg;
//		}
//		if(y_max >= 230){
//			y_max = avg;
//		}
//
//		if(y > y_max) {
//			y_max = y;
//		}
//
//		if(y < y_min) {
//			y_min = y;
//		}
//
//		threshold = (y_max + y_min) / 2;

		if(avg < threshold && status == 1){
			kroky++;
			status = 0;
		}else if(avg > threshold && status == 0){
			status = 1;
		}

		sprintf(send, "%d          %d          %d\r\n", avg, threshold, kroky);
//		sprintf(send, "%d\r\n", kroky);
		USARTp_start(send);

		for(int k=0;k<5000;k++){}

		//x_old = x;
		y_old = y;
		//z_old = z;
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
