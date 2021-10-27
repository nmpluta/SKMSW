/* Include header file depending upon part used */
#include <LPC21xx.H>

/* Macro Definitions */
#define SCL (1<<4)
#define SDA (1<<6)

#define AA (1<<2)
#define SI (1<<3)
#define STO (1<<4)
#define STA (1<<5)
#define I2EN (1<<6)

#define SCL_HIGH_DUTY_CYCLE 0x80
#define SCL_LOW_DUTY_CYCLE 0x80

#define I2C_VIC (1<<9)
#define I2C_VIC_NUMBER 9
#define VECTORED_IRQ_EN (1<<5)

/******************************************/
// Possible I2C status codes
/******************************************/
#define I2C_START_TRANSMITTED 0x08
#define I2C_REPEATED_START_TRANSMITTED 0x10
#define I2C_SLA_AND_W_ACKED 0x18
#define I2C_SLA_AND_W_NOT_ACKED 0x20
#define I2C_DATA_ACKED 0x28
#define I2C_DATA_NOT_ACKED 0x30
#define I2C_ARBITRATION_LOST 0x38

#define SLAVE_ADDR 0x10
#define WRITE_BIT 0

// start - inclusive, end - exclusive 
#define LAST(value, end) ((value) & ((1<<(end))-1))
#define MID(value, start, end) LAST((value)>>(start),((end)-(start)))

#define LED0_bm 1 << 16
#define LED1_bm 1 << 17
#define LED2_bm 1 << 18
#define LED3_bm 1 << 19
#define LED4_bm 1 << 20
#define LED5_bm 1 << 21
#define LED6_bm 1 << 22
#define LED7_bm 1 << 23

void I2C_Init(void);
void Delay(unsigned uiMiliSecs);
void PCF8574_Write(unsigned char ucData);
void Led_init(void);

unsigned char ucI2C_Data;

__irq void I2C_Interrupt(void)
{
	unsigned int uiI2C_Status;
	uiI2C_Status=I2STAT;
	
	switch(uiI2C_Status)
	{
		case I2C_START_TRANSMITTED:
			I2CONCLR = STA;
			I2DAT = (SLAVE_ADDR<<1)| WRITE_BIT; 
			break;

		case I2C_REPEATED_START_TRANSMITTED:
			I2CONCLR = STA;
			I2DAT = (SLAVE_ADDR<<1)| WRITE_BIT; 
			break;
		
		case I2C_SLA_AND_W_ACKED:
			I2CONCLR = AA;
			I2DAT = ucI2C_Data;
			break;

		// error
		case I2C_SLA_AND_W_NOT_ACKED:
			I2CONSET = STO;
			break;

		case I2C_DATA_ACKED:
			I2CONCLR = AA;
			I2CONSET = STO;
			break;

		// error
		case I2C_DATA_NOT_ACKED:
			I2CONSET = STO;
			break;

		// error
		case I2C_ARBITRATION_LOST:
			I2CONSET = STA;
			break;
		
		default:
			break;
	}
	I2CONCLR = SI;
	VICVectAddr = 0x0; // The ISR has finished!
}

/************************* MAIN *************************/
int main()
{
	unsigned char ucNum;
	
	Led_init();
	I2C_Init();
	while(1) {
		for(ucNum = 0; ucNum <= 255; ucNum++) 
		{
			PCF8574_Write(ucNum);
			Delay(50);
		}
	}
}
/*********************************************************/

void I2C_Init(void)
{
	/* Configure Pin Connect Block */
	PINSEL0 |= SCL|SDA;
	
	/* Clearing all flags */
	I2CONCLR = AA|SI|STA|I2EN;
	
	/* Enabling I2C */
	I2CONSET = I2EN;
	
	/* Set bit rate to  57.6Khz */
	I2SCLH = SCL_HIGH_DUTY_CYCLE;
	I2SCLL = SCL_LOW_DUTY_CYCLE;
	
	/* Initialize VIC for I2C use */
	VICIntSelect = 0x00;
	VICIntEnable = I2C_VIC;
	VICVectCntl0 = VECTORED_IRQ_EN | I2C_VIC_NUMBER;
	VICVectAddr0 = (unsigned long) I2C_Interrupt;
}

void PCF8574_Write(unsigned char ucData) 
{
	ucI2C_Data = ucData;
	I2CONSET = STA;
}

void Led_init(void) 
{
	
	IO1DIR = LED0_bm | LED1_bm | LED2_bm | LED3_bm | LED4_bm | LED5_bm | LED6_bm | LED7_bm;
	IO1CLR = LED0_bm | LED1_bm | LED2_bm | LED3_bm | LED4_bm | LED5_bm | LED6_bm | LED7_bm;
}

void Delay(unsigned uiMiliSecs)
{
	unsigned long ulCounter;
	for (ulCounter=0; ulCounter<(5455*uiMiliSecs); ulCounter++)
	{
		;
	}
}

