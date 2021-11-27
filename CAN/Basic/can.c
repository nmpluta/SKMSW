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
	PINSEL1 	|= 		CAN1_RX;			  				//Enable Pin 0.25 as CAN1 RX
	C1MOD 		= 		RM;											//Set CAN controller into reset
	C1BTR 		= 		BIT_TIMING_125k;				//Set bit timing to 125k
	C1TFI1 		= 		DLC_LEN_1_BYTE;					//Set DLC to 1 byte 
	C1CMR			= 		STB1;										// Select Tx Buffer 1 for transmission
	C1MOD 		= 		!RM;										//Release CAN controller
}

void Can2_InitAsRX(void)
{
	PINSEL1 	|= 		CAN2_RX|CAN2_TX;				// Enable Pin.023 as CAN2 RX
	C2MOD 		= 		RM;											//Set CAN controller into reset
	C2BTR 		= 		BIT_TIMING_125k;							//Set bit timing to 125k
	
	C2CMR 		= 		RRB;
	AFMR 			=			ACC_BP|ACC_OFF;					//Disable the Acceptance filters and accept all Rx msg
	C2MOD 		= 		!RM;										//Release CAN controller
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
	C1TID1 		= 		STD_11_BITS_FRAME;					//Set address to 0x22 Standard Frame
	C1TDA1 		= 		ucMsg;											// Data to transmit
	C1CMR 		|= 		TR;													// Transmission request
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
	C2CMR =	RRB; 													//release the recieve buffer
	return ucReceivedBytes;
}

