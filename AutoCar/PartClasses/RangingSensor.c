/*
 * RangingSensor.c
 *
 * Created: 4/13/2018 6:40:02 PM
 *  Author: Ryan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../HelperClasses/acx.h"
#include "../HelperClasses/acxserial.h"
#include "RangingSensor.h"
#include "../HelperClasses/sw_delay.h"

#define TRIG_PIN (1 << PB1)
#define ECHO_PIN (1 << PB0)

#define CM 58
#define IN 148
#define DIVISOR CM

static volatile uint16_t reading = 0;

void init_sensor() {
	cli();
	
	DDRB &= ~ECHO_PIN;	// set echo pin for input
    DDRB |= TRIG_PIN;	// set trigger pin for output

	// configure TIMER1 to increment its count at 4uS intervals
	TCCR1A = 0;
	TCCR1B = (1 << CS11) | (1 << CS10);	// prescaling = clk/64

	// enable pin change interrupts on ECHO_PIN
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT0);

	sei();
}

/* Return the number of cm (scaled by 100) away an object is.
 */
uint16_t read_sensor() {
	uint16_t interval;
	
	PORTB |= TRIG_PIN;
	delay_usec(10);
	PORTB &= ~TRIG_PIN;
	
	//while (!reading)
		//x_delay(1);
	while (!reading);
	
	interval = reading * 4;	// convert to uS
	reading = 0;
	return (interval * 10UL) / DIVISOR;
}

uint16_t read_avg_sensor(char num_readings) {
	uint32_t sum = 0;
	
	for (int i = 0; i < num_readings; i++) {
		sum += read_sensor();
	}
	
	return sum / num_readings;
}

uint16_t read_min_sensor(char num_readings) {
	uint16_t min, curr;
	
	min = read_sensor();
	
	for (int i = 1; i < num_readings; i++) {
		curr = read_sensor();
		min = curr < min ? curr : min;
	}
	
	return min;
}

ISR(PCINT0_vect) {
	static uint16_t rising_edge = 0;
	uint16_t curr = TCNT1;
	uint8_t line = PINB & ECHO_PIN;
	
	if (line)
		rising_edge = curr;
	else
		reading = curr - rising_edge;
}


