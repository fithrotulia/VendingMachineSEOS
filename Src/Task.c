/*
 * Task.c
 *
 *  Created on: Nov 24, 2019
 *      Author: fithrotul
 */
#include "main.h"
#include "Task.h"
#include "SEOS.h"

#define LED500 			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET)
#define LED500M 		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET)
#define LED1000			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)
#define LED1000M 		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)
#define LEDPROSES		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#define LEDPROSESM 		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LEDCANCEL		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET)
#define LEDCANCELM 		HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)

enum state {start, koin500, koin1000, proses, prosesdelay, cancel, canceldelay, out, outdelay, finish} Kondisi;
volatile uint64_t timeout;

unsigned char PB500(void);
unsigned char PB1000(void);
unsigned char PBPROSES(void);
unsigned char PBCANCEL(void);

void Task_Init(void)
{
	timeout=0;
	Kondisi=start;
}
void Task_Run(void)
{
	switch(Kondisi)
	{
	case start:
	{
		LED500M;
		LED1000M;
		LEDPROSESM;
		LEDCANCELM;
		Kondisi=koin500;
		break;
	}
	case koin500:
	{
		if(PB500())
		{
			LED500;
			LED1000M;
			LEDPROSESM;
			LEDCANCELM;
			Kondisi=koin1000;
		}
		if(PB1000())
		{
			LED1000;
			LED500M;
			LEDPROSESM;
			LEDCANCELM;
			Kondisi=proses;
		}
		break;
	}
	case koin1000:
	{
		if(PB500())
		{
			LED1000;
			LED500M;
			LEDPROSESM;
			LEDCANCELM;
			Kondisi=proses;
		}
		if(PB1000())
		{
			LED1000;
			LED500M;
			LEDPROSESM;
			LEDCANCELM;
			Kondisi=proses;
		}
		if(PBCANCEL())
		{
			Kondisi=cancel;
		}
		break;
	}
	case proses:
	{
		if(PBPROSES())
		{
			LEDPROSES;
			LED500M;
			LED1000M;
			LEDCANCELM;
			Kondisi=prosesdelay;
		}
		if(PBCANCEL())
		{
			Kondisi=cancel;
		}
		break;
	}
	case prosesdelay:
	{
		if(++timeout>1000000)
		{
			timeout=0;
			Kondisi=out;
		}
		break;
	}
	case cancel:
	{
		LED500M;
		LED1000M;
		LEDPROSESM;
		LEDCANCEL;
		Kondisi=canceldelay;
		break;
	}
	case canceldelay:
	{
		if(++timeout>1000000)
		{
			timeout=0;
			Kondisi=finish;
		}
		break;
	}
	case out:
	{
		LED500;
		LED1000;
		LEDPROSES;
		LEDCANCEL;
		Kondisi=outdelay;
	}
	case outdelay:
	{
		if(++timeout>1000000)
		{
			timeout=0;
			Kondisi=finish;
		}
		break;
	}
	case finish:
	{
		Kondisi=start;
	}
	}
}

unsigned char PB500(void)
{
	static unsigned char bouncing=0xFF;
	unsigned char Flagdetect=0;
	if(HAL_GPIO_ReadPin(PB500_GPIO_Port, PB500_Pin)==GPIO_PIN_RESET)
	{
		bouncing=(bouncing<<1);
	}
	else
	{
		bouncing=(bouncing<<1)|1;
		timeout=0;
	}

	if(bouncing==0x03)
	{
		Flagdetect=1;
	}
	return Flagdetect;
}
unsigned char PB1000(void)
{
	static unsigned char bouncing=0xFF;
	unsigned char Flagdetect=0;
	if(HAL_GPIO_ReadPin(PB1000_GPIO_Port, PB1000_Pin)==GPIO_PIN_RESET)
	{
		bouncing=(bouncing<<1);
	}
	else
	{
		bouncing=(bouncing<<1)|1;
		timeout=0;
	}

	if(bouncing==0x03)
	{
		Flagdetect=1;
	}
	return Flagdetect;
}
unsigned char PBPROSES(void)
{
	static unsigned char bouncing=0xFF;
	unsigned char Flagdetect=0;
	if(HAL_GPIO_ReadPin(PBPROSES_GPIO_Port, PBPROSES_Pin)==GPIO_PIN_RESET)
	{
		bouncing=(bouncing<<1);
	}
	else
	{
		bouncing=(bouncing<<1)|1;
		timeout=0;
	}

	if(bouncing==0x03)
	{
		Flagdetect=1;
	}
	return Flagdetect;
}
unsigned char PBCANCEL(void)
{
	static unsigned char bouncing=0xFF;
	unsigned char Flagdetect=0;
	if(HAL_GPIO_ReadPin(PBCANCEL_GPIO_Port, PBCANCEL_Pin)==GPIO_PIN_RESET)
	{
		bouncing=(bouncing<<1);
	}
	else
	{
		bouncing=(bouncing<<1)|1;
		timeout=0;
	}

	if(bouncing==0x03)
	{
		Flagdetect=1;
	}
	return Flagdetect;
}
