
#include <avr/io.h>
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include "LCD.h"
#include <stdio.h>
#include <string.h>

#define F3    2.8641
#define Gb3   2.7027
#define G3    2.5510
#define Ab3   2.4087
#define A3    2.2727
#define Bb3   2.1435
#define B3    2.0248



#define C4    1.9097
#define Db4   1.8052
#define D4    1.7024
#define Eb4   1.6079
#define E4    1.5181
#define F4    1.4323
#define Gb4   1.3514
#define G4    1.2755
#define Ab4   1.2036
#define A4    1.1364
#define Bb4   1.0724
#define B4    1.0126


#define C5    0.9028
#define Db5   0.8514
#define D5    0.8037
#define Eb5   0.7589
#define E5    0.7163


// Buzzer Configurations
#define BUZZER_DDR      DDRB
#define BUZZER_PORT     PORTB
#define BUZZER_PIN      3

void Play_Sound(float frequency);
void stop_sound(void);


/*74HC165 read function*/
volatile uint8_t readInput1(void){
	uint8_t reader = 0;

	/* Set PINC2 (PC2) as input */
	DDRC &= ~(1 << PC2);

	/* Raise LD pin */
	PORTC |= (1 << PC1);

	for (int i = 0; i < 8; i++){
		reader <<= 1;   /* Get a one-bit digital input */
		reader |= ((PINC & (1 << PC2)) ? 1 : 0);
		
		/* Clock the register */
		PORTC &= ~(1 << PC0);
		_delay_us(1);
		PORTC |= (1 << PC0);
	}

	/* Lower the LD pin */
	PORTC &= ~(1 << PC1);

	return reader;
}

volatile uint8_t readInput2(void){
	uint8_t reader = 0;

	/* Set PIND3 (PD3) as input */
	DDRD &= ~(1 << PD3);

	/* Raise LD pin */
	PORTC |= (1 << PC1);

	for (int i = 0; i < 8; i++){
		reader <<= 1;   /* Get a one-bit digital input */
		reader |= ((PIND & (1 << PD3)) ? 1 : 0);
		
		/* Clock the register */
		PORTC &= ~(1 << PC0);
		_delay_us(1);
		PORTC |= (1 << PC0);
	}

	/* Lower the LD pin */
	PORTC &= ~(1 << PC1);

	return reader;
}

volatile uint8_t readInput3(void){
	uint8_t reader = 0;

	/* Set PIND2 (PD2) as input */
	DDRD &= ~(1 << PD2);

	/* Raise LD pin */
	PORTC |= (1 << PC1);

	for (int i = 0; i < 8; i++){
		reader <<= 1;   /* Get a one-bit digital input */
		reader |= ((PIND & (1 << PD2)) ? 1 : 0);
		
		/* Clock the register */
		PORTC &= ~(1 << PC0);
		_delay_us(1); // You can use delay functions for a more accurate timing
		PORTC |= (1 << PC0);
	}

	/* Lower the LD pin */
	PORTC &= ~(1 << PC1);

	return reader;
}
int main(void)
{
lcd_init();
lcd_clear();
_delay_ms (50);
lcd_goto_xy(0, 0);
lcd_write_word("Toy Piano Music!");
	Play_Sound_new(F3);

	Play_Sound_new(Gb3);

	Play_Sound_new(G3);

	Play_Sound_new(Ab3);

	Play_Sound_new(A3);

	Play_Sound_new(Bb3);

	Play_Sound_new(B3);

	Play_Sound_new(C4);
	Play_Sound_new(Db4);

	Play_Sound_new(D4);

	Play_Sound_new(Eb4);

	Play_Sound_new(E4);

	Play_Sound_new(F4);

	Play_Sound_new(Gb4);

Play_Sound_new(G4);

Play_Sound_new(Ab4);

Play_Sound_new(A4);

Play_Sound_new(Bb4);

Play_Sound_new(B4);

Play_Sound_new(C5);

Play_Sound_new(Db5);

Play_Sound_new(D5);

Play_Sound_new(Eb5);

Play_Sound_new(E5);


_delay_ms(2000);


Play_Sound(F3);

Play_Sound(Gb3);

Play_Sound(G3);

Play_Sound(Ab3);

Play_Sound(A3);

Play_Sound(Bb3);

Play_Sound(B3);

Play_Sound(C4);
Play_Sound(Db4);

Play_Sound(D4);

Play_Sound(Eb4);

Play_Sound(E4);

Play_Sound(F4);

Play_Sound(Gb4);

Play_Sound(G4);

Play_Sound(Ab4);

Play_Sound(A4);

Play_Sound(Bb4);

Play_Sound(B4);

Play_Sound(C5);

Play_Sound(Db5);

Play_Sound(D5);

Play_Sound(Eb5);

Play_Sound(E5);


	/* Initialize PORTC and set PC0, PC1, PC2 as output */
	DDRC |= 0b11111111;
	DDRD |= 0b11111111;
	while (1){
		/* Read the input value */
		uint8_t switchNo = readInput1();
		uint8_t switchNo2 = readInput2();
		uint8_t switchNo3 = readInput3();
		
		float freq = 0;
		
		if(PINB & 0B00000001){
			/* Iterate through each bit of switchNo */
			for (int i = 0; i < 8; i++) {
				/* Check if the i-th bit of switchNo is set */
				if (switchNo & (1 << i)) {
					/* Set frequency based on which button is pressed */
					switch (i) {
						case 0:freq += F3;break;
						case 1:freq += Gb3;break;
						case 2:freq += G3;break;
						case 3:freq += Ab3;break;
						case 4:freq += A3;break;
						case 5:freq += Bb3;break;
						case 6:freq += B3;break;
						case 7:freq += C4;break;
						default:break;
					}
				}
			}
			
			
			
			for (int i = 0; i < 8; i++) {
				/* Check if the i-th bit of switchNo is set */
				if (switchNo2 & (1 << i)) {
					/* Set frequency based on which button is pressed */
					switch (i) {
						case 0:freq += Db4;break;
						case 1:freq += D4;break;
						case 2:freq += Eb4;break;
						case 3:freq += E4;break;
						case 4:freq += F4;break;
						case 5:freq += Gb4;break;
						case 6:freq += G4;break;
						case 7:freq += Ab4;break;
						default:break;
					}
				}
			}
			
			
			for (int i = 0; i < 8; i++) {
				/* Check if the i-th bit of switchNo is set */
				if (switchNo3 & (1 << i)) {
					/* Set frequency based on which button is pressed */
					switch (i) {
						case 0:freq += A4;break;
						case 1:freq += Bb4;break;
						case 2:freq += B4;break;
						case 3:freq += C5;break;
						case 4:freq += Db5;break;
						case 5:freq += D5;break;
						case 6:freq += Eb5;break;
						case 7:freq += E5;break;
						default:
						break;
					}
				}
			}
			}
		
		
		if (freq > 0) {
			Play_Sound(freq);
			}else{
			stop_sound();
		}
		

	}
}

#include <math.h>

#define PI 3.14159265

#define SAMPLE_RATE     30000  // Adjust as needed


void Play_Sound_new(float frequency)
{
	float duration=5;
	long int i,cycles;
	float wavelength = 1.25;

	cycles=duration/wavelength;

	BUZZER_DDR |= (1 << BUZZER_PIN);


	for (i = 0; i < 10; i++)
	{

		for(int j=0;j<20;j++){
			_delay_ms(frequency / 10.5);
			BUZZER_PORT |= (1 << BUZZER_PIN);
			_delay_ms(22 * frequency / 10.5); // Ensuring the same duty cycle for both on and off times
			BUZZER_PORT &= ~(1 << BUZZER_PIN);
		}
		for(int j=0;j<10;j++){
			_delay_ms(frequency / 10.55);
			BUZZER_PORT |= (1 << BUZZER_PIN);
			_delay_ms(22.1 * frequency / 10.55); // Ensuring the same duty cycle for both on and off times
			BUZZER_PORT &= ~(1 << BUZZER_PIN);
		}
		
		for(int j=0;j<5;j++){
			_delay_ms(frequency / 10.6);
			BUZZER_PORT |= (1 << BUZZER_PIN);
			_delay_ms(22.2 * frequency / 10.6); // Ensuring the same duty cycle for both on and off times
			BUZZER_PORT &= ~(1 << BUZZER_PIN);
		}
		for(int j=0;j<10;j++){
			_delay_ms(frequency / 10.55);
			BUZZER_PORT |= (1 << BUZZER_PIN);
			_delay_ms(22.1 * frequency / 10.55); // Ensuring the same duty cycle for both on and off times
			BUZZER_PORT &= ~(1 << BUZZER_PIN);
		}

	}

}

void Play_Sound(float frequency)
{
	for(int i=0;i<200;i++){
		float duration=5;
		long int i,cycles;
		float wavelength = 1.25;
		
		cycles=duration/wavelength;

		BUZZER_DDR |= (1 << BUZZER_PIN);

		for (i=0;i<cycles;i++)
		{
			_delay_ms(frequency);
			BUZZER_PORT |= (1 << BUZZER_PIN);
			_delay_ms(frequency);
			BUZZER_PORT &= ~(1 << BUZZER_PIN);
		}
	}
	
}


void stop_sound(void)
{
	// Stop sound
	BUZZER_PORT &= ~(1 << BUZZER_PIN);
}