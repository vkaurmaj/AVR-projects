#include "avr.h"
#include "lcd.h"
#include <stdio.h>

#define TARGET_PORT PORTC
#define TARGET_DDR DDRC
#define TARGET_VALUE PINC


struct tm {
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minutes;
	unsigned int seconds;
	char morning[3];
	char night[3];
	int day_or_night;
};

void advance_time(struct tm * t) {
	++t->seconds;
	
	// accounts for incrementation
	if (60 <= t->seconds) {
		++t->minutes;
		t->seconds = 0;
	}
	if (60 <= t->minutes) {
		++t->hour;
		t->minutes = 0;
	}
	
	if (12 <= t->hour && t->day_or_night == 1) {
		++t->day;
		t-> hour = 1;	
	}
	
	if (12 <= t->hour && t->day_or_night == 0) {
		++t->day;
		t->hour = 0;
	}
	
	
	if (31 <= t->day) {
		++t->month;
		t->day = 1;
	}
	if (12 <= t->month) {
		++t->year;
		t->month = 1;
	}
}

void display_time(struct tm t) {
	char date[17];
	char time[17];
	sprintf(date, "%02d/%02d/%04d", t.month, t.day, t.year);
	if (t.day_or_night) {
		if (12 <= t.hour) {
			sprintf(time, "%02d:%02d:%02d %s", t.hour, t.minutes, t.seconds, t.night);
			t.day_or_night = 0;			
		}
		 else {
			sprintf(time, "%02d:%02d:%02d %s", t.hour, t.minutes, t.seconds, t.morning);
		}
		
	} else if (!(t.day_or_night)) {
		if (12 <= t.hour) {
			sprintf(time, "%02d:%02d:%02d %s", t.hour, t.minutes, t.seconds, t.morning);
			t.day_or_night = 1;
		}
		else {
			sprintf(time, "%02d:%02d:%02d %s", t.hour, t.minutes, t.seconds, t.night);
		}	
	}
	

	lcd_pos(0, 0);
	lcd_puts2(date);
	lcd_pos(1,0);
	lcd_puts2(time);
	
}

int get_key()
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
	if (keycode == 0xEE) return 1;
	if (keycode == 0xDE) return 2;
	if (keycode == 0xBE) return 3;
	if (keycode == 0xED) return 4;
	if (keycode == 0xDD) return 5;
	if (keycode == 0xBD) return 6;
	if (keycode == 0xEB) return 7;
	if (keycode == 0xDB) return 8;
	if (keycode == 0xBB) return 9;
	if (keycode == 0xD7) return 10;
	
	// * key
	if (keycode == 0xE7) return 11;
	// # key
	if (keycode == 0xB7) return 12;
	
	// A key
	if (keycode == 0x7E) return 13;
	
	// B key
	if (keycode == 0x7D) return 14;

	// C key
	if (keycode == 0x7B) return 15;
	
	// D key
	if (keycode == 0x77) return 16;
	
	return -1;
}

void process_key(int k, struct tm * t) {
	switch(k) {
			
		case 1:
		    // increment month
			++t->month;
			break;
		case 2:
			// increment day
			++t->day;
			break;
		case 3:
			// increment year
			++t->year;
			break;
		case 4:		
			// decrement month
			--t->month;
			break;
		case 5:
			// decrement day
			--t->day;
			break;		
		case 6: 
			// decrement year
			--t->year;
			break;
		case 7:
			// increment hour
			++t->hour;
			break;
		case 8:
			// increment minute
			++t->minutes;
			t->seconds = 0;
			break;
		case 9:
			// increment second
			++t->seconds;
			break;
		case 11:		
			// decrement hour
			--t->hour;
			break;
		case 10:
			// decrement minute
			--t->minutes;
			break;		
		case 12: 
			// decrement second
			--t->seconds;
			break;
						
			
		default:
			break;		
	}
	display_time(*t);
	return;
}


int main(void)
{
	lcd_init();
	struct tm time = {2019,12,31,11,0,0,"AM","PM",0};
	TARGET_DDR = 0x0F;
	TARGET_PORT = 0xF0;
	
	while (1) {
		int k = get_key();
		process_key(k, &time);
		display_time(time);
		advance_time(&time);
		avr_wait(1000);
	}
}
