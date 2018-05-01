/*
 * PhotoInterruptor.c
 *
 * Created: 4/19/2018 6:17:43 PM
 *  Author: Ryan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../HelperClasses/acx.h"
#include "PhotoInterruptor.h"

#define LINPUT_PIN  (1 << PD1)
#define RINPUT_PIN  (1 << PD2)
#define LINT_PIN    (1 << INT1)
#define RINT_PIN    (1 << INT2)

/* rotary encoder disc:
 *     radius: 3.3cm
 *     theta: (2pi/NUM_SLOTS): .314159    
 */
#define NUM_SLOTS  20	// number of slots in the rotary encoder
#define CIRC	   207	// circumference: 20.7cm (scaled by 10)
#define STEP	   10	// arc length b/w slots: 1.0cm (scaled by 10)

volatile static unsigned int lsteps = 0;
volatile static unsigned int rsteps = 0;


/* init_photoInterruptors - Initialize the pin settings and interrupts for the 
 *     wheels' photo interrupters.
 */
void init_photoInterruptors() {
	cli();
	
	// set pins for input
	DDRD &= ~(LINPUT_PIN | RINPUT_PIN);

	// enable external interrupts on input pins
	EICRA |= (1 << ISC11) | (1 << ISC21);	// interrupt on falling edge
	EIMSK |= (1 << INT1) | (1 << INT2);
	
	sei();
}

void reset_steps() {
	lsteps = 0;
	rsteps = 0;
}

void reset_lsteps() {
	lsteps = 0;
}

void reset_rsteps() {
	rsteps = 0;
}

void get_steps(unsigned int *left, unsigned int *right) {
	*left = lsteps;
	*right = rsteps;
}

unsigned int get_lsteps() {
	return lsteps;
}

unsigned int get_rsteps() {
	return rsteps;
}

unsigned int stepsToCm(unsigned int steps) {
	unsigned int cm = 0;
	
	while (steps >= NUM_SLOTS) {
		steps -= NUM_SLOTS;
		cm += CIRC;
	}
	
	cm += steps * STEP;
	return cm;
}

unsigned int cmToSteps(unsigned int cm) {
	unsigned int steps = 0;
	
	while (cm >= CIRC) {
		cm -= CIRC;
		steps += NUM_SLOTS;
	}
	
	steps += cm / STEP;
	return steps;
}

/* LINPUT_PIN external interrupt - Update left wheel time/distance
*     measurements on falling edges of LINPUT_PIN.
 */
ISR(INT1_vect) {
	++lsteps;
}

/* RINPUT_PIN external interrupt - Update right wheel time/distance 
 *     measurements on falling edges of RINPUT_PIN.
 */
ISR(INT2_vect) {
	++rsteps;
}
