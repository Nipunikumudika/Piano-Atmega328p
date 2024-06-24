
#include <avr/io.h>
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "hd44780pcf8574.h"
#define INTERRUPT_PIN_PD3 PD3
#define INTERRUPT_PIN_PD5 PD5


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

#define USART_BAUDRATE 9600 // Desired Baud Rate
#define BAUD_PRESCALER (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define ASYNCHRONOUS (0<<UMSEL00) // USART Mode Selection
#define PARITY_MODE  (0<<UPM00) // USART Parity Bit Selection
#define STOP_BIT (0<<USBS0)     // USART Stop Bit Selection
#define DATA_BIT   (3<<UCSZ00)  // USART Data Bit Selection

char addr = PCF8574_ADDRESS;

void USART_Init()
{
	// Set Baud Rate
	UBRR0H = BAUD_PRESCALER >> 8;
	UBRR0L = BAUD_PRESCALER;
	
	// Set Frame Format
	UCSR0C = ASYNCHRONOUS | PARITY_MODE | STOP_BIT | DATA_BIT;
	
	// Enable Receiver and Transmitter
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
}


void USART_TransmitChar(uint8_t DataByte) {
	while (!(UCSR0A & (1<<UDRE0))); // Wait until UDR is ready
	UDR0 = DataByte;
}

void USART_TransmitString(char *str) {
	while (*str) {
		USART_TransmitChar(*str);
		str++;
	}
}

void USART_TransmitInteger(int value) {
	char buffer[10]; // Assuming a maximum of 10 digits for the integer
	sprintf(buffer, "%d", value);
	USART_TransmitString(buffer);
}

void USART_TransmitFixedPoint(float value) {
	char buffer[10]; // Assuming a maximum of 10 digits for the integer
	sprintf(buffer, "%d.%04u ", (int)value , (int) ((value*10000)-(int)value*10000) );
	USART_TransmitString(buffer);
}


void Play_Sound_violin(float frequency)
{

	BUZZER_DDR |= (1 << BUZZER_PIN);


	for(int j=0;j<20;j++){
		_delay_ms(frequency / 10.5);
		BUZZER_PORT |= (1 << BUZZER_PIN);
		_delay_ms(22 * frequency / 10.5);
		BUZZER_PORT &= ~(1 << BUZZER_PIN);
	}
	for(int j=0;j<10;j++){
		_delay_ms(frequency / 10.55);
		BUZZER_PORT |= (1 << BUZZER_PIN);
		_delay_ms(22.1 * frequency / 10.55);
		BUZZER_PORT &= ~(1 << BUZZER_PIN);
	}
	
	for(int j=0;j<5;j++){
		_delay_ms(frequency / 10.6);
		BUZZER_PORT |= (1 << BUZZER_PIN);
		_delay_ms(22.2 * frequency / 10.6);
		BUZZER_PORT &= ~(1 << BUZZER_PIN);
	}
	for(int j=0;j<10;j++){
		_delay_ms(frequency / 10.55);
		BUZZER_PORT |= (1 << BUZZER_PIN);
		_delay_ms(22.1 * frequency / 10.55);
		BUZZER_PORT &= ~(1 << BUZZER_PIN);
	}

	
}

void Play_Sound_piano(float frequency)
{

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


void stop_sound(void)
{
	// Stop sound
	BUZZER_PORT &= ~(1 << BUZZER_PIN);
}


/*74HC165 read function*/
volatile uint8_t readInput1(void){
	uint8_t reader = 0;

	//Set PINC2 (PC2) as input 
	DDRC &= ~(1 << PC2);

	//Raise LD pin 
	PORTC |= (1 << PC1);

	for (int i = 0; i < 8; i++){
		reader <<= 1;  // Get a one-bit digital input 
		reader |= ((PINC & (1 << PC2)) ? 1 : 0);
		
		 //Clock the register 
		PORTC &= ~(1 << PC0);
		_delay_us(1);
		PORTC |= (1 << PC0);
	} 

	//Lower the LD pin 
	PORTC &= ~(1 << PC1);

	return reader;
}


volatile uint8_t readInput2(void){
	uint8_t reader = 0;

	/* Set PINB4 (PB4) as input */
	DDRB &= ~(1 << PB4);

	/* Raise LD pin */
	PORTC |= (1 << PC1);

	for (int i = 0; i < 8; i++){
		reader <<= 1;   /* Get a one-bit digital input */
		reader |= ((PINB & (1 << PB4)) ? 1 : 0);

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

	/* Set PINB4 (PB5) as input */
	DDRB &= ~(1 << PB5);

	/* Raise LD pin */
	PORTC |= (1 << PC1);

	for (int i = 0; i < 8; i++){
		reader <<= 1;   /* Get a one-bit digital input */
		reader |= ((PINB & (1 << PB5)) ? 1 : 0);

		/* Clock the register */
		PORTC &= ~(1 << PC0);
		_delay_us(1);
		PORTC |= (1 << PC0);
	}

	/* Lower the LD pin */
	PORTC &= ~(1 << PC1);

	return reader;
}




volatile float calculateFrequency(void){
	float freq = 0;
	/* Read the input value */
	uint8_t switchNo = readInput1();
	uint8_t switchNo2 = readInput2();
	uint8_t switchNo3 = readInput3();

	for (int i = 0; i < 8; i++) {
		//Check if the i-th bit of switchNo is set
		if (switchNo & (1 << i)) {
			// Set frequency based on which button is pressed 
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
		//Check if the i-th bit of switchNo is set
		if (switchNo2 & (1 << i)) {
			//Set frequency based on which button is pressed
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
		// Check if the i-th bit of switchNo is set
		if (switchNo3 & (1 << i)) {
			//Set frequency based on which button is pressed
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

	return freq;
}

uint8_t instrument = -1;
uint8_t bluetooth = -1;

// Define BADISR_vect to handle unhandled interrupts
ISR(BADISR_vect)
{
	for (;;) {	}
}

ISR( INT0_vect )
{
	_delay_ms(50);
	if (PIND & (1 << PD2)) {
		instrument=0;
		HD44780_PCF8574_PositionXY(addr, 0, 1);
		HD44780_PCF8574_DrawString(addr, "Play Piano......");
		}else{
		instrument=1;
		HD44780_PCF8574_PositionXY(addr, 0, 1);
		HD44780_PCF8574_DrawString(addr, "Play Violin.....");
		
	}
	
}

ISR( INT1_vect )
{
	_delay_ms(50);
	if (PIND & (1 << PD3)) {
		HD44780_PCF8574_PositionXY(addr, 0, 0);
		HD44780_PCF8574_DrawString(addr, "Bluetooth.......");
		USART_Init();
		bluetooth=1;
		}else{
		HD44780_PCF8574_PositionXY(addr, 0, 0);
		HD44780_PCF8574_DrawString(addr, "Toy Piano Music!");
		bluetooth=0;
	}
}

int main(void)
{
	PORTC |= (1 << PC2);  // Enable pull-up on PC2
	PORTB |= (1 << PB4);  // Enable pull-up on PB4
	PORTB |= (1 << PB5);  // Enable pull-up on PB5

	stop_sound();
	
	
	HD44780_PCF8574_Init(addr);
	//display clear
	HD44780_PCF8574_DisplayClear(addr);
	//display on
	HD44780_PCF8574_DisplayOn(addr);
	
for(int i=0;i<20;i++){
Play_Sound_piano(2);}
for(int i=0;i<20;i++){
Play_Sound_piano(2.5);}
for(int i=0;i<20;i++){
Play_Sound_piano(4);}
stop_sound();
	
	DDRC |= 0b11111111;
	DDRD |= 0b11111111;
	
	EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
	EIMSK |= (1 << INT0);     // Turns on INT0
	
	EICRA |= (1 << ISC10);    // set INT1 to trigger on ANY logic change
	EIMSK |= (1 << INT1);     // Turns on INT1
	
	if (PIND & (1 << PD2)) {
		instrument=0;
		HD44780_PCF8574_PositionXY(addr, 0, 1);
		HD44780_PCF8574_DrawString(addr, "Play Piano......");
		}else{
		instrument=1;
		// position
		HD44780_PCF8574_PositionXY(addr, 0, 1);
		HD44780_PCF8574_DrawString(addr, "Play Violin.....");
		
	}
	if (PIND & (1 << PD3)) {
		HD44780_PCF8574_PositionXY(addr, 0, 0);
		HD44780_PCF8574_DrawString(addr, "Bluetooth.......");
		USART_Init();
		bluetooth=1;
		}else{
		HD44780_PCF8574_PositionXY(addr, 0, 0);
		HD44780_PCF8574_DrawString(addr, "Toy Piano Music!");
		bluetooth=0;
	}
	sei();			/* Enable Global Interrupt */
	float frequecy=0;
	
	
	while (1){
		float frequecy = calculateFrequency();
		
		if (frequecy > 0) {
			
			if(instrument==0 && bluetooth==0){
				Play_Sound_piano(frequecy);
			}
			else if(instrument==1 && bluetooth==0){
				Play_Sound_violin(frequecy);
			}
			else if(bluetooth==1){
				stop_sound();
				USART_TransmitInteger(instrument);
				USART_TransmitChar(' ');
				USART_TransmitFixedPoint(frequecy);
				USART_TransmitChar('\r'); // Carriage return
				USART_TransmitChar('\n');
			}
			}else{
			stop_sound();
		}
	}
}
