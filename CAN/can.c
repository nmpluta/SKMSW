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
#define RBS 1 << 0

void Led_Init(void)
{
	IO1DIR = LED0_bm | LED1_bm | LED2_bm | LED3_bm | LED4_bm | LED5_bm | LED6_bm | LED7_bm;
	IO1CLR = LED0_bm | LED1_bm | LED2_bm | LED3_bm | LED4_bm | LED5_bm | LED6_bm | LED7_bm;
}

void Led_ShowByte(unsigned char ucLedsOn){

}

void Can1_InitAsTX(void)
{
	PINSEL1 	|= 		CAN1_RX;			  //Enable Pin 0.25 as CAN1 RX
	C1MOD 		= 		0x00000001;			//Set CAN controller into reset
	C1BTR 		= 		0x001C001D;			//Set bit timing to 125k
	C1MOD 		= 		0x00000000;			//Release CAN controller

	C1TFI1 		= 		0x00040000;			//Set DLC to 4 bytes
	C1CMR			= 		STB1;						// Select Tx Buffer 1 for transmission
}

void Can2_InitAsRX(void)
{
	PINSEL1 |= CAN2_RX | CAN2_TX;							// Enable Pin.023 as CAN2 RX
	C2MOD 		= 		0x00000001;			//Set CAN controller into reset
	C2BTR 		= 		0x001C001D;			//Set bit timing to 125k

	AFMR =	0x00000003;						//Disable the Acceptance filters and accept all Rx msg
	C2CMR = RRB;
	C2MOD 		= 		0x00000000;			//Release CAN controller
}


unsigned int ucCan1_TxFree(void)
{
	C1TID1 		= 		0x00000002;			//Set address to 2 Standard Frame
	if(C1SR & 0x00000004)
	{
		IO1SET = LED0_bm;
		return 1;
	}
	else
		return 0;
}

void Can1_SendByte(unsigned char ucMsg)
{
	C1TID1 = ID_11BITS; 
	C1TDA1 = ucMsg;								// Data to transmit
	C1CMR 		= 		0x00000021;			//Transmit the message
	//C1CMR |= TR;									// Transmission request
}

unsigned int ucCan2_RxReady(void)
{
	if(C2SR & RBS)
	{
		IO1SET = LED1_bm;
		return 1;
	}
	else
		return 0;
}

unsigned char ucCan2_ReceiveByte(void)
{
	unsigned char ucReceivedBytes = C2RDA;
	C2CMR =	RRB; 					//release the recieve buffer
	return ucReceivedBytes;
}
