/*
 * GccApplication3.c
 *
 * Created: 1/22/2020 9:06:41 PM
 * Author : Vahagn Kaurmajyan
 */ 

#include "avr.h"


int main(void)
{
	
	SET_BIT(DDRD, PD7); 
		
    while (1) 
    {
		PORTD ^= (1 << PD7); // Uses bitwise XOR to flip PD7 on and off
		avr_wait(500);
    }
	
	return 0;
}

