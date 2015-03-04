/*
 * Next_Make_CPW_2015.c
 *
 * Created: 2/28/2015 10:54:56 PM
 *  Author: sanoj737
 */ 


#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>

// num is a number from 0 to 255
void displayAsBinary(int num)
{
	PORTA = 0b11111110;
	PORTB = 0b00000110;
	if(num >= 128) {
		PORTB &= 0b11111011;
		num -= 128;
	}
	if(num >= 64) {
		PORTB &= 0b11111101;
		num -= 64;
	}
	if(num >= 32) {
		PORTA &= 0b01111110;
		num -= 32;
	}
	if(num >= 16) {
		PORTA &= 0b10111110;
		num -= 16;
	}
	if(num >= 8) {
		PORTA &= 0b11011110;
		num -= 8;
	}
	if(num >= 4) {
		PORTA &= 0b11101110;
		num -= 4;
	}
	if(num >= 2) {
		PORTA &= 0b11110110;
		num -= 2;
	}
	if(num >= 1) {
		PORTA &= 0b11111010;
		num -= 1;
	}
}

int main(void)
{
	// These set whether our pins are input (0) or output (1) for port A and B
	DDRA = 0b11111110;
	DDRB = 0b00000110;
	
	// ADC enable
	ADCSRA |= 1 << ADEN;
	
	// ADC start conversion
	ADCSRA |= 1 << ADSC;
	
	// ADC auto trigger enable
	ADCSRA |= 1 << ADATE;
	
	// Divide clock frequency by 128 because ADCs freak out at 8MHz
	ADCSRA |= 0b00000111;
	
	// Enable right shift (throws out 2 least significant digits!)
	ADCSRB |= 1 << ADLAR;
	
	// Our initial temperature output. Stripy!
	float output = 0b10101010;
	
	// Temperature constants for the thermometer IC
	float TC = 0.0195;
	float V0C = 0.400;
	
	// TA = ambient temperature in degrees C, which is what we want to display
	float TA;
	
    while(1)
    {
		// All hail Lord Sanoj
		// Sanoj, our supreme leader
		// In Sanoj we trust
		
		// ADCH should grab the temperature
		// Do a moving average because it looks gross otherwise
		output = 0.5 * output + 0.5 * ADCH;
		
		// VOUT = TC*TA + V0C
		// TA = (VOUT - V0C) / TC
		// We also need to convert the output back to a voltage
		TA = ((output / 255 * 5) - V0C) / TC;
		displayAsBinary((int)(TA));
		_delay_us(100000);
    }
}