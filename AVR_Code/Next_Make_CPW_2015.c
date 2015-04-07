/*
 * Next_Make_CPW_2015.c
 *
 * Created: 2/28/2015 10:54:56 PM
 *  Author: sanoj737
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

// num is a number from 0 to 255
void displayAsBinary(int num)
{
	/* PORTA = 0b11111110;
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
	} */
	
	int tempA = 0b11111110;
	int tempB = 0b00000111;
	
	//PORTA = 0b11111110;
	//PORTB = 0b00000111;
	
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

void welcome(void)
{
	// All hail Lord Sanoj
	// Sanoj, our supreme leader
	// In Sanoj we trust
	
	//'A' = 65
	//'l' = 108
	//'H' = 72
	//'a' = 97
	//'i' = 105
	//'L' = 76
	//'o' = 111
	//'r' = 114
	//'d' = 100
	//'S' = 83
	//'n' = 110
	//'j' = 106
	//',' = 44
	//'u' = 117
	//'r' = 114
	//'s' = 115
	//'u' = 117
	//'p' = 112
	//'n' = 110
	//'m' = 109
	//'w' = 119
	
	//int UNDER_NINE_THOUSAND = 50;
	//int MESSAGE_CHAR[] = {65, 108, 108, 104, 97, 105, 108, 76, 111, 114, 100, 83, 97, 110, 111, 106, 83, 97, 110, 111, 106, 44, 111, 117, 114, 115, 117, 112, 114, 
	//101, 109, 101, 108, 101, 97, 100, 101, 114, 73, 110, 83, 97, 110, 111, 106, 119, 101, 116, 114, 117, 115, 116};
	
	//for(int index = 0; index < sizeof(MESSAGE_CHAR); index++)
	//{
	//	displayAsBinary(MESSAGE_CHAR[index]);
	//	_delay_ms(UNDER_NINE_THOUSAND);
	//}
}

void increment(void)
{
	for(int displayInt = 0; displayInt < 1024; displayInt++){
		displayAsBinary(displayInt)	;
		_delay_us(50000);
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
	
	// Enable right shift (throws out 2 least significant digits!)
	//ADCSRB |= 1 << ADLAR;
	
	// Our initial temperature output. Stripy!
	float output = 0b1010101010;
	int rawInput = 0;
	
	// Temperature constants for the thermometer IC
	//float TC = 0.0195;
	//float V0C = 0.400;
	
	// TA = ambient temperature in degrees C, which is what we want to display
	float TA;
	//int TA_int;
	
	// Welcome message
	//welcome();
	
	//display all LEDs to signify start
	displayAsBinary(0b111111111);
	_delay_us(1000000);
	
	
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
		//TA_int = (int)(TA*1000);
		TA = TA * 51.28;
		//TA = ((output / 931.0) - V0C) / TC;

		displayAsBinary((int)(TA*10.0));
		
		_delay_us(10000);
		
		//displayAsBinary( (int)(output / 1023.0 * 5.0 * 250.0) );
		//displayAsBinary( (int)(rawInput) );
		//displayAsBinary(0b11111111);
    }
}