#include "avr.h"
#include <util/delay.h>

void
avr_init(void)
{
  /* WDTCR = 15; uncomment this if you know how Watch Dog Timers work */
}

void
avr_wait(unsigned short msec)
{
  TCCR0 = 3;
  while (msec--) {
    TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.000001);
    SET_BIT(TIFR, TOV0);
    WDR();
    while (!GET_BIT(TIFR, TOV0));
  }
  TCCR0 = 0;
}

void avr_wait_us(unsigned short usec) {
	while(usec--) {
		_delay_us(1);

	}
}
