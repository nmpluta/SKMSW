#include <LPC21xx.H>
#include "can_lo.h"

#define CAN_RD1				0x01 << 18
#define CAN_RD2				0x01 << 14
#define CAN_TD2				0x01 << 16

#define RM 						0x01 << 0
#define TBS1					0x01 << 2
#define DLC_1_BYTE		0x01 << 16

#define RIE						0x01 << 0
#define VIC_CAN2RX_CHANNEL_NR 27
#define mIRQ_SLOT_ENABLE 0x01 << 5

#define ACC_OFF 			0x01 << 0
#define ACC_BP				0x01 << 1
#define RRB						0x01 << 2

#define TX_FREE				0x01 << 2
#define RX_AV					0x01 << 0
#define ID_11BITS			0x0000022
#define TX_REQUEST		0x01 << 0


#define LED_CLEAR     0x0000005

#define LED0_bm 1 << 16
#define LED1_bm 1 << 17
#define LED2_bm 1 << 18
#define LED3_bm 1 << 19
#define LED4_bm 1 << 20
#define LED5_bm 1 << 21
#define LED6_bm 1 << 22
#define LED7_bm 1 << 23

void Led_Init(void) 
{
	IO1DIR = LED0_bm | LED1_bm | LED2_bm | LED3_bm | LED4_bm | LED5_bm | LED6_bm | LED7_bm;
	IO1CLR = LED0_bm | LED1_bm | LED2_bm | LED3_bm | LED4_bm | LED5_bm | LED6_bm | LED7_bm;
}

void Led_ShowByte(unsigned char ucLedsOn){
	
}

 void Can1_InitAsTX( void )
 {
		PINSEL1 |= CAN_RD1 ;
		C1MOD = RM; 
		C1BTR = 0x001C001D; 
		C1TFI1 = DLC_1_BYTE;
		C1MOD &= (~RM); 
 }

void Can2_InitAsRX(void)
 {
	 	PINSEL1 |= CAN_RD2 | CAN_TD2 ;
		C2MOD = RM; 
		C2BTR = 0x001C001D; 
		AFMR = ACC_OFF | ACC_BP;
		C2CMR = RRB;
		C2MOD &= (~RM); 
	 
 }
 
 unsigned char ucCan1_TxFree(void)
 {
	 	if (C1SR & TX_FREE)
		{ 
			IO1SET = LED0_bm;
			return 1;
		}
		else
			return 0; 
 }
 
 
 unsigned char ucCan2_RxReady(void)
 {
	 	if(C2SR & RX_AV)
		{
			IO1SET = LED1_bm;
			return 1;
		}
		else
			return 0;
 }
 
void Can1_SendByte(unsigned char ucData)
 {
		C1TID1 = ID_11BITS; 
		C1TDA1 = ucData;
		C1CMR  = TX_REQUEST;
 }
 
unsigned char ucCan2_ReceiveByte(void)
 {
		C2CMR = RRB;
	 	return C2RDA;
 }
 