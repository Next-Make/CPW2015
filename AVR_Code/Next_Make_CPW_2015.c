/*
 * Next_Make_CPW_2015.c
 *
 * Created: 2/28/2015 10:54:56 PM
 * Author: Next-Makers
 */ 


#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>


//PA0 connects to temperature sensor
//PB2 (LSB)
//PB1
//PA7
//PA6
//PA5
//PA4
//PA3
//PA2
//PA1
//PB0 (MSB)

// num is a number from 0 to 1023
void displayAsBinary(int num)
{
	int tempA = 0b11111110;
	int tempB = 0b00000111;
	
	if(num>511){
		num = num - 512;
		tempB &= ~(1 << 0);
	}
	
	for(int leftShift = 8; leftShift >= 0; leftShift--){
		if(num >= 256){
			if(leftShift < 2){
				tempB &= ~(1 << (2 - leftShift));
			}else{
				tempA &= ~(1 << (9 - leftShift ));
			}	
			num -= 256;
		}
		num = num << 1;
	}
	
	PORTA = tempA;
	PORTB = tempB;
}

void runCoolAnimation(void)
{
	int animationVal = 1;

	for(int count = 0; count < 10; count++){
		displayAsBinary(animationVal);
		_delay_ms(50);

		animationVal = animationVal << 1;
	}

	for(int count = 0; count < 10; count++){
		animationVal = animationVal >> 1;

		displayAsBinary(animationVal);
		_delay_ms(50);
	}

	int animationSeries[10] = {0b1000000001, 0b0100000010, 0b0010000100, 0b0001001000, 0b0000110000, 0b0001001000, 0b0010000100, 0b0100000010, 0b1000000001, 0b0000000000};

	for(int seriesIndex = 0; seriesIndex < 10; seriesIndex++){
		displayAsBinary(animationSeries[seriesIndex]);
		_delay_ms(50);
	}
}

int main(void)
{
	// These set whether our pins are input (0) or output (1) for port A and B
	DDRA = 0b11111110;
	DDRB = 0b00000111;
	
	// Divide clock frequency by 128 because ADCs freak out at 8MHz
	ADCSRA |= 0b00000111;
	
	// ADC auto trigger enable
	ADCSRA |= 1 << ADATE;
	
	// ADC enable
	ADCSRA |= 1 << ADEN;
		
	// ADC start conversion
	ADCSRA |= 1 << ADSC;
	
	//set internal 1.1V as ADC reference
	ADMUX |= 1 << REFS1;
	
	// Our initial temperature output. Stripy!
	float output = 0b1010101010;
	int rawInput = 0;
	
	// TA = ambient temperature in degrees C, which is what we want to display
	float TA;
	
	// display all LEDs to signify start
	displayAsBinary(0b111111111);
	_delay_us(50);
	
	displayAsBinary(0b000000000);
	_delay_us(50);

	// cool animation
	runCoolAnimation();

    while(1)
    {	
		// ADCH should grab the temperature
		// Do a moving average because it looks gross otherwise
		rawInput = ADCL;
		rawInput = rawInput + (ADCH<<8);
		output = 0.9 * output + 0.1 * rawInput;
		
		//VOUT = TC*TA + V0C
		// TA = (VOUT - V0C) / TC
		// We also need to convert the output back to a voltage
		TA = output/1024.0;
		TA = TA*2.2;
		TA = TA - 0.4;
		TA = TA * 51.28;

		displayAsBinary((int)(TA*10.0));
		
		_delay_us(10000);
    }
}