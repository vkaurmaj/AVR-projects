#include "avr.h"
#include "lcd.h"
#include <stdio.h>

#define TARGET_PORT PORTC
#define TARGET_DDR DDRC
#define TARGET_VALUE PINC

unsigned short max=0;
unsigned short min=9999;
unsigned average=0;
unsigned long  long total=0;
int count=0;

unsigned char keypress()
{
		// if no keys are pressed, break the function
		if (TARGET_VALUE == 0xF0) {
			return -1;
		}
		// save column value
		uint8_t keycode = TARGET_VALUE;
		
		// reverse i/o to get output
		TARGET_DDR ^= 0xFF;
		TARGET_PORT ^= 0xFF;
		
		// wait to avoid duplicated key presses
		// and for i/o inversion to register
		avr_wait(200);
		
		// get full value for key press
		keycode |= TARGET_VALUE;
		
		// register key press accordingly
		
		// A key
		if (keycode == 0x7E) return 1;
		
		// B key
		if (keycode == 0x7D) return 2;
		
		return -1;
}

unsigned short sample_calc()
{
	// Configure ADC Analog Ref
	SET_BIT(ADCSRA,7);
	SET_BIT(ADCSRA,6);
	// Waits for conversion
	while(!(ADCSRA & (1<<ADIF)));
	unsigned short result = ADC/10.23*5;
	
	// adds up all
	total+=result;
	// keeps count of how many samples taken
	count++;
	
	// sets min/max values
	if(result>max)
	{
		max=result;
	}
	if(result<min)
	{
		min=result;
	}
	// calculates average
	average=total/count;
	CLR_BIT(ADCSRA,7);
	return result;
}

int main(void)
{
	lcd_init();
	lcd_clr();
	// initialize decimal value and floating point
	// value for each field
	int ivA = 0;
	int ivB = 0;
	int avgA = 0;
	int avgB = 0;
	int maxA = 0;
	int maxB = 0;
	int minA = 0;
	int minB = 0;
	
	char menu[17];
	unsigned short result = 1;
	
	TARGET_DDR = 0x0F;
	TARGET_PORT = 0xF0;

	SET_BIT(ADMUX,6);
	ADCSRA|=135;
	
	while(1)
	{
		lcd_pos(0,0);
		result = sample_calc();
		ivA = result / 100;
		ivB = result % 100;
		sprintf(menu,"IV:%01d.%02d", ivA, ivB);	
		lcd_puts2(menu);
		lcd_pos(1, 0);
		sprintf(menu,"A: Show more");
		lcd_puts2(menu);
		if(keypress() == 1)
		{
			while(1)
			{
				if(keypress() == 2)
				{
					result = 1;
					max=0;
					min=9999;
					average=0;
					total=0;
					count=0;
					lcd_clr();
					break;
				}
				result = sample_calc();
				lcd_clr();
				lcd_pos(0, 0);
				ivA = result / 100;
				ivB = result % 100;
				avgA = average / 100;
				avgB = average % 100;
				maxA = max / 100;
				maxB = max % 100;
				minA= min / 100;
				minB = min % 100;
				sprintf(menu,"IV:%01d.%02d Mx:%01d.%02d",ivA,ivB,maxA,maxB);
				lcd_puts2(menu);
				lcd_pos(1, 0);
				sprintf(menu,"Avg:%01d.%02d Mn:%01d.%02d",avgA,avgB,minA,minB);
				lcd_puts2(menu);
				avr_wait(500);
			}
		}
		
		avr_wait(500);
		
	}
}

