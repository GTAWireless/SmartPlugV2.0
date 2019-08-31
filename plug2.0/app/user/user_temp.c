/*
 * user_temp.c
 *
 *  Created on: 2018骞�11鏈�17鏃�
 *      Author: lenovo
 */

#include "user_common.h"
#include "esp_common.h"


/*
	温度                                   电阻                             AD
 ----------------------------------
	-10   		44.12
	-9    		42.11
	-8    		40.21
	-7    		38.39
	-6    		36.67
	-5    		35.03
	-4   		33.48
	-3		    32   
	-2    		30.6 
	-1    		29.27
	 0    		28.01 
	 1    		26.82 
	 2    		25.69 
	 3    		24.62 
	 4   		23.61 
	 5    		22.65 
	 6    		21.75 
	 7    		20.89 
	 8    		20.07 
	 9    		19.29 
	10    		18.56
	11    		18.48
	12    		18.14
	13    		17.63
	14    		16.99
	15    		16.27
	16    		15.53
	17    		14.78
	18    		14.05
	19    		13.35
	20    		12.69
	21    		12.06
	22    		11.49
	23    		10.95
	24    		10.45
	25    		10   
	26    		9.57 
	27    		9.18 
	28    		8.81 
	29    		8.47 
	30    		8.16 
	31    		7.86 
	32    		7.57 
	33    		7.31 
	34    		7.05 
	35    		6.81 
	36    		6.58 
	37    		6.35 
	38    		6.14 
	39    		5.93 
	40    		5.73 
	41    		5.54 
	42    		5.35 
	43    		5.17 
	44    		4.99 
	45    		4.82 
	46    		4.66 
	47    		4.5  
	48    		4.35 
	49    		4.2  
	50    		4.06 
	51    		3.92 
	52    		3.79 
	53    		3.66 
	54    		3.53 
	55    		3.41 
	56    		3.29 
	57    		3.17 
	58   		3.05 
	59    		2.94 
	60    		2.82 
	61    		2.77 
	62    		2.71 
	63    		2.65 
	64    		2.58 
	65    		2.5  
	66    		2.43 
	67    		2.35 
	68    		2.28 
	69    		2.2  
	70    		2.13 
	71    		2.06 
	72   		2   
	73    		1.93 
	74    		1.87 
	75    		1.82 
	76    		1.76 
	77    		1.71 
	78    		1.67 
	79    		1.62 
	80    		1.58 



    R          AD
  ----- = -----------
  R热 + R   1024 * 3.3

=>
      1024 * 3.3 * R
 R热 = ---------------- - R
             AD
=>
R = 1K

        1024 * 3.3
 R热  = --------------  - 1
            AD


 */

const INT8 TempTab[] =
{
	-9, -8, -7, -6, -5, -4, -3, -2, -1,  0,
	1,   2,  3,  4,  5,  6,  7,  8,  9, 10,
	11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
	31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
	51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
	61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
	71, 72, 73, 74, 75, 76, 77, 78, 79, 80
};

const float ResistorTab[] =
{
	42.11, 40.21, 38.39, 36.67, 35.03, 33.48, 32.00, 30.60, 29.27, 28.01,
	26.82, 25.69, 24.62, 23.61, 22.65, 21.75, 20.89, 20.07, 19.29, 18.56,
	18.48, 18.14, 17.63, 16.99, 16.27, 15.53, 14.78, 14.05, 13.35, 12.69,
	12.06, 11.49, 10.95, 10.45, 10.00,  9.57,  9.18,  8.81,  8.47,  8.16,
	 7.86,  7.57,  7.31,  7.05,  6.81,  6.58,  6.35,  6.14,  5.93,  5.73,
	 5.54,  5.35,  5.17,  4.99,  4.82,  4.66,  4.50,  4.35,  4.20,  4.06,
	 3.92,  3.79,  3.66,  3.53,  3.41,  3.29,  3.17,  3.05,  2.94,  2.82,
	 2.77,  2.71,  2.65,  2.58,  2.50,  2.43,  2.35,  2.28,  2.20,  2.13,
	 2.06,  2.00,  1.93,  1.87,  1.82,  1.76,  1.71,  1.67,  1.62,  1.58
};

extern float MCU_GetTemperature( VOID );

/*
VOID TEMP_TempCallBack( VOID )
{
	STATIC UINT uiLastAdc = 0;
	UINT uiCurAdc = 0;

	if ( uiLastAdc == 0 )
	{
		uiLastAdc = system_adc_read();
	}
	portENTER_CRITICAL();
	uiCurAdc = system_adc_read();
	portEXIT_CRITICAL();

	uiTempAdcValue = ( uiLastAdc * 0.8 +  uiCurAdc * 0.2 );
	uiLastAdc = uiTempAdcValue;
}

*/


float TEMP_GetTemperature( VOID )
{
#if 0
	STATIC UINT uiLastAdc = 0;
	UINT uiCurAdc = 0;
	float fResistor = 0;
	UINT8 i = 0;
	float fTemp = 0;

	portENTER_CRITICAL();
	if ( uiLastAdc == 0 )
	{
		uiLastAdc = system_adc_read();
	}
	uiCurAdc = system_adc_read();
	portEXIT_CRITICAL();

	uiCurAdc = ( uiLastAdc + uiCurAdc )/2;
	uiLastAdc = uiCurAdc;

	fResistor = (float)((1024.0 * 3.3 / uiCurAdc ) - 1.0 );

	for( i = 0; i < sizeof(ResistorTab)/sizeof(ResistorTab[0]); i++ )
	{
		if ( fResistor > ResistorTab[i] )
		{
			break;
		}
	}

	if ( i >= sizeof(ResistorTab)/sizeof(ResistorTab[0]) )
	{
		fTemp = 999;
	}
	else
	{
		fTemp = TempTab[i-1] + ( ResistorTab[i-1] - fResistor ) / ( ResistorTab[i-1] - ResistorTab[i] )*(TempTab[i]-TempTab[i-1]);
	}
	//LOG_OUT(LOGOUT_INFO, "fTemp:%f, fResistor:%f", fTemp, fResistor);
	return fTemp;
#else

	return MCU_GetTemperature();

#endif
}


