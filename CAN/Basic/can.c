#include <LPC21xx.H>
#include "can.h"

#define LED0_bm 1 << 16
#define LED1_bm 1 << 17
#define LED2_bm 1 << 18
#define LED3_bm 1 << 19
#define LED4_bm 1 << 20
#define LED5_bm 1 << 21
#define LED6_bm 1 << 22
#define LED7_bm 1 << 23

#define CAN1_RX 1 << 18
#define CAN2_RX 1 << 14
#define CAN2_TX 1 << 16
#define STB1 1 << 5
#define TR 1 << 0
#define RRB 1 << 2
#define ACC_OFF 1 << 0
#define ACC_BP 1 << 1
#define RBS_FREE 1 << 0
#define TBS1_FREE 1 << 2
#define RM 1 << 0;
#define DLC_LEN_1_BYTE 1 << 16
#define BIT_TIMING_125k 0x001C001D
#define STD_11_BITS_FRAME 0x22

void Led_Init(void) 
{
	IO1DIR = LED0_bm | LED1_bm | LED2_bm | LED3_bm | LED4_bm | LED5_bm | LED6_bm | LED7_bm;
	IO1CLR = LED0_bm | LED1_bm | LED2_bm | LED3_bm | LED4_bm | LED5_bm | LED6_bm | LED7_bm;
}

void Led_ShowByte(unsigned char ucLedsOn){
	IO1CLR = LED0_bm | LED1_bm | LED2_bm | LED3_bm | LED4_bm | LED5_bm | LED6_bm | LED7_bm;
	IO1SET = ucLedsOn << 16;
}

void Can1_InitAsTX(void)
{
	PINSEL1 	|= 		CAN1_RX;
	C1MOD 		= 		RM;
	C1BTR 		= 		BIT_TIMING_125k;
	C1TFI1 		= 		DLC_LEN_1_BYTE;
	C1CMR			= 		STB1;
	C1MOD 		= 		!RM;
}

void Can2_InitAsRX(void)
{
	PINSEL1 	|= 		CAN2_RX|CAN2_TX;
	C2MOD 		= 		RM;
	C2BTR 		= 		BIT_TIMING_125k;
	
	C2CMR 		= 		RRB;
	AFMR 			=			ACC_BP|ACC_OFF;
	C2MOD 		= 		!RM;
}


unsigned char ucCan1_TxFree(void)
{
	if(C1SR & TBS1_FREE)
		return 1;
	else 
		return 0;
}

void Can1_SendByte(unsigned char ucMsg)
{
	C1TID1 		= 		STD_11_BITS_FRAME;
	C1TDA1 		= 		ucMsg;
	C1CMR 		|= 		TR;
}

unsigned char ucCan2_RxReady(void)
{
	if(C2SR & RBS_FREE)
		return 1;

	else
		return 0;
}

unsigned char ucCan2_ReceiveByte(void)
{
	unsigned char ucReceivedBytes = C2RDA;
	C2CMR =	RRB;
	return ucReceivedBytes;
}

