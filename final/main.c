/*
 * Final!.c
 *
 * Created: 4/30/2025 11:41:18 PM
 * Author : Marcus
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

static inline void initADC0(void){
	ADMUX = (1 << REFS0); // AVCC as reference, ADC0 input
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
	ADCSRA |= (1 << ADEN); // Enable ADC
}

int main(void)
{
	uint8_t ledValue;
	uint16_t adcValue;
	uint8_t i;

	initADC0();

	DDRB = 0x3F;  // PB0–PB5 as output for LEDs
	DDRD |= (1 << PD6) | (1 << PD2) | (1 << PD3); // PD6: PWM, PD2/PD3: motor direction

	// Set motor direction: IN1 high, IN2 low (forward)
	PORTD |= (1 << PD2);
	PORTD &= ~(1 << PD3);

	// Configure Timer0 for Fast PWM on PD6 (OC0A)
	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); // Fast PWM
	TCCR0B |= (1 << CS01) | (1 << CS00);                   // Prescaler 64

	while (1)
	{
		// Read ADC value
		ADCSRA |= (1 << ADSC);
		loop_until_bit_is_clear(ADCSRA, ADSC);
		adcValue = ADC;

		// LED bar level (0–7 range)
		ledValue = adcValue >> 7;

		// Clear all LEDs on PORTB
		PORTB &= ~0x3F; // 63 in decimal to run A0-A5 for the 6 LEDs

		// Light up LEDs based on potentiometer level
		for (i = 0; i <= ledValue && i < 6; i++) {
			PORTB |= (1 << i);
		}

		// Set PWM duty cycle (0–255)
		OCR0A = adcValue >> 2;

		_delay_ms(50);
	}
}

