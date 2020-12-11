/*
 * project5.c
 *
 * Created: 3/19/2020 8:36:05 PM
 * Author : Krikor Andonian
 */ 

#include "avr.h"
#include "lcd.h"
#include <stdio.h>
#include <stdbool.h>

#define TARGET_PORT PORTC
#define TARGET_DDR DDRC
#define TARGET_VALUE PINC

int Num1,Num2,Number;
char key,action;
bool result = false;

char get_key()
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
	
	// integers 0-9
	if (keycode == 0xEE) return '1';
	if (keycode == 0xDE) return '2';
	if (keycode == 0xBE) return '3';
	if (keycode == 0xED) return '4';
	if (keycode == 0xDD) return '5';
	if (keycode == 0xBD) return '6';
	if (keycode == 0xEB) return '7';
	if (keycode == 0xDB) return '8';
	if (keycode == 0xBB) return '9';
	if (keycode == 0xD7) return '10';
	
	// * key
	if (keycode == 0xE7) return '*';
	// # key
	if (keycode == 0xB7) return '#';
	
	// A key
	if (keycode == 0x7E) return 'A';
	
	// B key
	if (keycode == 0x7D) return 'B';

	// C key
	if (keycode == 0x7B) return 'C';
	
	// D key
	if (keycode == 0x77) return 'D';
	
	return 'X';
}

void buttons()
{
	if (key=='*') //If cancel Button is pressed
		Number=Num1=Num2=0; result=false;
	
	if (key == '1') { //If Button 1 is pressed
		if (Number==0)
			Number=1;
		else
			Number = (Number*10) + 1; //Pressed twice
	}
	
	if (key == '4') { //If Button 4 is pressed
		if (Number==0)
			Number=4;
		else
			Number = (Number*10) + 4; //Pressed twice
	}
	
	if (key == '7') { //If Button 7 is pressed
		if (Number==0)
			Number=7;
		else
			Number = (Number*10) + 7; //Pressed twice
	}
	

	if (key == '0') {//Button 0 is Pressed
		if (Number==0)
		Number=0;
		else
		Number = (Number*10) + 0; //Pressed twice
	}
	
	if (key == '2') { //Button 2 is Pressed
		if (Number==0)
			Number=2;
		else
			Number = (Number*10) + 2; //Pressed twice
	}
	
	if (key == '5') {
		if (Number==0)
			Number=5;
		else
			Number = (Number*10) + 5; //Pressed twice
	}
	
	if (key == '8') {
		if (Number==0)
			Number=8;
		else
			Number = (Number*10) + 8; //Pressed twice
	}

	if (key == '#') {
		Num2=Number;
		result = true;
	}
	
	if (key == '3'){
		if (Number==0)
			Number=3;
		else
			Number = (Number*10) + 3; //Pressed twice
	}
	
	if (key == '6') {
		if (Number==0)
			Number=6;
		else
			Number = (Number*10) + 6; //Pressed twice
	}
	
	if (key == '9') {
		if (Number==0)
			Number=9;
		else
			Number = (Number*10) + 9; //Pressed twice
	}

	if (key == 'A' || key == 'B' || key == 'C' || key == 'D') { //Detecting operator
		Num1 = Number;
		Number = 0;
		if (key == 'A') action = '+';
		if (key == 'B') action = '-'; 
		if (key == 'C') action = '*';
		if (key == 'D') action = '/';
		lcd_clr();
		avr_wait(100);
	}

	
}

void calc()
{
	if (action=='+')
	Number = Num1+Num2;

	if (action=='-')
	Number = Num1-Num2;

	if (action=='*')
	Number = Num1*Num2;

	if (action=='/')
	Number = Num1/Num2;
}

void display()
{
	
	char buf[16]; // need a buffer for that
	
	sprintf(buf,"%d",Num1);
	const char *p = buf;
	lcd_pos(0, 0);   // set the cursor to column 0, line 1
	lcd_puts2(buf);
	sprintf(buf,"%c", action);
	lcd_puts2(p); 
	sprintf(buf,"%d",Num2);
	lcd_puts2(p);
	
	if (result==true) {
		lcd_puts2("="); 
		lcd_pos(1,0);   // set the cursor to column 0, line 1
		sprintf(buf,"%d",Number);
		lcd_puts2(p); //Display the result
	} //Display the results
	
	if (Number != 0) {
		lcd_pos(1,0);   // set the cursor to column 0, line 1
		sprintf(buf,"%d",Number);
		lcd_puts2(p); //Display the result
	}
	
}

int main(void) {
	lcd_init();	
	TARGET_DDR = 0x0F;
	TARGET_PORT = 0xF0;
	while(true) {
		key = get_key(); //storing pressed key value in a char
		if (key == '*') 
			lcd_clr();
		
		if (key!='X')
			buttons();

		if (result==true)
			calc();

			display();
	}
}

