/*
 * BetterMotorPWMV1.c
 *
 * Created: 4/1/2025 5:42:18 PM
 * Author : Marcus
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

//define global variables
volatile uint8_t phase = 0;
volatile int ovrFloCt = 0;

static inline void initTimer0(void){
	TCCR0B |= (1 << CS01 | 1 << CS00);
	TIMSK0 |= (1 << OCIE0B);
	TIMSK0 |= (1 << TOIE0);
	sei();
}

//AFTER TIMER HITS 255, RUNS OVF
ISR(TIMER0_OVF_vect){
	PORTD |= 4;
	ovrFloCt++;
	if(ovrFloCt == 9800){
		if(phase == 0){
			phase = 1;
			OCR0B = 128;
		} else if(phase ==1){
			phase = 2;
			OCR0B = 191;
		} else if(phase ==2){
			phase = 3;
			OCR0B = 64;
		} else if(phase ==3){
			phase = 0;
			OCR0B = 255;
		}
		ovrFloCt = 0;
	}
	
}

//WHEN TIMER HITS OCR0B VALUE
ISR(TIMER0_COMPB_vect){
	PORTD &= ~4;
}

int main(void)
{
    DDRB |= 4;
	initTimer0();
    while (1) 
    {
    }
}

