/* Include header file depending upon part used */
#include <LPC21xx.H>
#include <math.h>



/* Macro Definitions */
#define SPIF (1<<7)
#define SCK (1<<8)
#define MISO (1<<10)
#define MOSI (1<<12)
#define CS (1<<10) // PIN0.10 as CS
#define MSTR (1<<5)


#define CONFIG_DATA 0x3
#define MAX_VOLTAGE 3300
#define MAX_HEX_VOLTAGE 0xFFF
#define PI 3.14159265



// start - inclusive, end - exclusive
#define LAST(value, end) ((value) & ((1<<(end))-1))
#define MID(value, start, end) LAST((value)>>(start),((end)-(start)))

static unsigned short sin_LUT[360];
struct SPI_FrameParams
{
	unsigned char ucCpha;
	unsigned char ucCpol;
	unsigned char ucClsbf;
	unsigned char ClkDivider;
};

void DAC_MCP4921_Set_mV(unsigned int uiVoltage);
void DAC_MCP4921_Set(unsigned int uiVoltage);
void SPI_ConfigMaster(struct SPI_FrameParams params);
void Initialize(void);
unsigned int DAC_MCP4921_sin_Get(unsigned int uiAngle);
void Delay(unsigned uiMiliSecs);




/************************* MAIN *************************/
int main()
{
		short uiAngle;
	  for ( uiAngle = 0; uiAngle < 360; uiAngle++)
    {
			sin_LUT[uiAngle] = ((sin(uiAngle*PI/180))+1)*1000;
    }
    Initialize();
    while (1)
    {
        for ( uiAngle = 0; uiAngle < 360; uiAngle++)
        {
            DAC_MCP4921_Set_mV(sin_LUT[uiAngle]);
        }
    }
}

/*********************************************************/


void DAC_MCP4921_Set_mV(unsigned int uiVoltage)
{
    unsigned int uiScaledVoltage;
    if (uiVoltage > MAX_VOLTAGE)
    {
        uiScaledVoltage = MAX_HEX_VOLTAGE;
    } else
    {
        uiScaledVoltage = (uiVoltage * MAX_HEX_VOLTAGE) / MAX_VOLTAGE;
    }
    DAC_MCP4921_Set(uiScaledVoltage);
}


void DAC_MCP4921_Set(unsigned int uiVoltage)
{
    IO0CLR = CS;
/* Write out confing data and MSB bits*/
    S0SPDR = (CONFIG_DATA << 4) | MID(uiVoltage, 8, 12);
/* Wait for transfer to be completed */
    while (!(S0SPSR & SPIF)) {}
/* Write LSB bits out */
    S0SPDR = MID(uiVoltage, 0, 8);
/* Wait for transfer to be completed */
    while (!(S0SPSR & SPIF)) {}
    IO0SET = CS;
}


void SPI_ConfigMaster(struct SPI_FrameParams params)
{
	
}

void Initialize()
{
/* Configure Pin Connect Block */
    PINSEL0 |= SCK | MISO | MOSI;

/* Configure P0.10 as output pin (Chip Select) */
    IO0DIR |= CS;

/* Device selected as master */
    S0SPCR |= MSTR;

/* Set pclk to same as cclk */
    VPBDIV = 0x1;
/* Set to highest speed for SPI at 10 MHz- > 1.25 MHz */
    S0SPCCR = 0x8;
}
