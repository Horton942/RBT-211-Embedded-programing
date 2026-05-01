/*
 * potMeterV1.c
 *
 * Created: 4/8/2025 6:06:17 PM
 * Author : Marcus
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

static inline void initADC0(void){
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS2 | ADPS1 | ADPS0);
	ADCSRA |= (1 << ADEN);
}

int main(void)
{
	// make variables
	uint8_t ledValue;
	uint16_t adcValue;
	uint8_t i;
	
	//initialize adc
	initADC0();
	
	//set D as Outputs
	DDRD = 0xff;
    while (1) 
    {
		//start ADC collection
		ADCSRA |= (1 << ADSC);
		loop_until_bit_is_clear(ADCSRA, ADSC);
		adcValue = ADC;
		ledValue = (adcValue >> 7);
		
		
		PORTD = 0;
		for (i = 0; i <= ledValue; i++) {
			PORTD |= (1 << i);
    }
	_delay_ms(50);
}
}

