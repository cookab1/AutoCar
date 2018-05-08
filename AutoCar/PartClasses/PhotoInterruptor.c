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

/* rotary encoder disc:
 *     radius: 3.25cm
 *     theta: (2pi/NUM_SLOTS): .314159    
 */
#define NUM_SLOTS  20	// number of slots in the rotary encoder
#define CIRC	   204	// circumference: 20.4cm (scaled by 10)
#define STEP	   10	// arc length b/w slots: 1.0cm (scaled by 10)

#define LINPUT_PIN  (1 << PJ0)
#define RINPUT_PIN  (1 << PJ1)
#define LINT_PIN    (1 << PCINT9)
#define RINT_PIN    (1 << PCINT10)

static volatile unsigned int lsteps = 0;
static volatile unsigned int rsteps = 0;


/* init_photoInterruptors - Initialize the pin settings and interrupts for the 
 *     wheels' photo interrupters.
 */
void init_photoInterruptors() {
	cli();
	
	// set pins for input
	DDRJ &= ~(LINPUT_PIN | RINPUT_PIN);
	
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (LINT_PIN | RINT_PIN);
	
	sei();
}

void reset_steps() {
	PCICR &= ~(1 << PCIE1);
	
	lsteps = 0;
	rsteps = 0;
	
	PCICR |= (1 << PCIE1);
}

void reset_lsteps() {
	lsteps = 0;
}

void reset_rsteps() {
	rsteps = 0;
}

void get_steps(unsigned int *left, unsigned int *right) {
	PCICR &= ~(1 << PCIE1);
	
	*left = lsteps;
	*right = rsteps;
	
	PCICR |= (1 << PCIE1);
}

unsigned int get_lsteps() {
	return lsteps;
}

unsigned int get_rsteps() {
	return rsteps;
}

unsigned int stepsToCm(unsigned int steps) {
	unsigned int revs, rem;
	
	revs = steps / NUM_SLOTS;
	rem = steps - (revs * NUM_SLOTS);
	
	return (revs * CIRC) + (rem * STEP);
}

unsigned int cmToSteps(unsigned int cm) {
	unsigned int revs, rem;
	
	revs = cm / CIRC;
	rem = cm - (revs * CIRC);
	
	return (revs * NUM_SLOTS) + (rem / STEP);
}

ISR(PCINT1_vect) {
	static char left = 0;
	static char right = 0;
	char newleft, newright;
	
	newleft = PINJ & LINPUT_PIN;
	newright = PINJ & RINPUT_PIN;
	
	/* update counts on falling edges */
	if (left && !newleft)
		lsteps++;
	if (right && !newright)
		rsteps++;
	
	left = newleft;
	right = newright;
}
