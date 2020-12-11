#include "avr.h"
#include "lcd.h"

// used for keypad inputs
#define TARGET_PORT PORTC
#define TARGET_DDR DDRC
#define TARGET_VALUE PINC

// half periods (us) of notes using
// their frequencies
#define D5 851
#define E5 758
#define Fsh5 675
#define G5 637
#define A5 568
#define B5 506
#define C6 477
#define D6 425
#define DUR 40

int get_key() {
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
	
	// 'A' key
	if (keycode == 0x7E) return 1;
	
	return -1;
}

void play_note(uint16_t delay, uint8_t duration) {
	// delay is half-period in us
	// duration is in 10ms increments	
	
	//            V - this 10ms in microseconds
	// cycles = 10000 * duration / delay
	// cycles = 100 * duration / (delay/100)
	uint16_t cycles = (100 * duration) / (delay/100);
	
	while(cycles > 0) {
		SET_BIT(PORTA, PA1);
		avr_wait_us(delay);
		CLR_BIT(PORTA, PA1);
		avr_wait_us(delay);
		cycles--;
	}
}

void playHappyBday() {
	play_note(D5, DUR);
	if (get_key() == 1) return;
	play_note(E5, DUR);
	if (get_key() == 1) return;
	play_note(D5, DUR);
	if (get_key() == 1) return;
	play_note(G5, DUR);
	if (get_key() == 1) return;
	play_note(Fsh5, 2*DUR);
	if (get_key() == 1) return;
	play_note(D5, DUR);
	if (get_key() == 1) return;
	play_note(E5, DUR);
	if (get_key() == 1) return;
	play_note(D5, DUR);
	if (get_key() == 1) return;
	play_note(A5, DUR);
	if (get_key() == 1) return;
	play_note(G5, 2*DUR);
	if (get_key() == 1) return;
	play_note(D5, DUR);
	if (get_key() == 1) return;
	play_note(D6, DUR);
	if (get_key() == 1) return;
	play_note(B5, DUR);
	if (get_key() == 1) return;
	play_note(G5, DUR);
	if (get_key() == 1) return;
	play_note(Fsh5, DUR);
	if (get_key() == 1) return;
	play_note(E5, DUR);
	if (get_key() == 1) return;
	play_note(C6, DUR);
	if (get_key() == 1) return;
	play_note(B5, DUR);
	if (get_key() == 1) return;
	play_note(G5, DUR);
	if (get_key() == 1) return;
	play_note(A5, DUR);
	if (get_key() == 1) return;
	play_note(G5, 2*DUR);
	if (get_key() == 1) return;
}


int main() {
	// int happyBday[21] = {D5, E5, D5, G5, Fsh5, D5, 
	//						E5, D5, A5, G5, D5, D6, B5, 
	//						G5, Fsh5, E5, C6, B5, G5, A5, G5 }
	
	// set pin A1 to output and keypad i/o
	SET_BIT(DDRA, PA1);	
	TARGET_DDR = 0x0F;
	TARGET_PORT = 0xF0;
	// fire up the LCD & display song name / volume
	lcd_init();
	lcd_puts2("Happy Birthday!");
	lcd_pos(1,0);
	lcd_puts2("Volume: ***");
	
	int startStop = 0;
	
	// play notes!
	while(1) {
		if (startStop == 0 && get_key() == 1) {
			startStop = 1;
		}
		else if (startStop == 1 && get_key() == 1) {
			startStop = 0;
		}
		else if (startStop == 0 && get_key() != 1) {
			playHappyBday();
		}		
		// delay a bit
		avr_wait(500);

	}
	
	return 0;
}

