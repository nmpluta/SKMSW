#include <LPC21xx.H>
void Led_Init(void);
void Led_ShowByte(unsigned char ucLedsOn);

void Can1_InitAsTX(void);
void Can2_InitAsRX(void);

unsigned char ucCan1_TxFree(void);
unsigned char ucCan2_RxReady(void);

void Can1_SendByte(unsigned char ucMsg);
unsigned char ucCan2_ReceiveByte(void);



