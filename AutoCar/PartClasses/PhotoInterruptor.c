/*
 * PhotoInterruptor.c
 *
 * Created: 4/19/2018 6:17:43 PM
 *  Author: Ryan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "acx.h"
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
#define SEGMENT	   10	// arc length b/w slots: 1.0cm (scaled by 10)

static rotaryDisc_t left = { 0, 0, 0 };
static rotaryDisc_t right = { 0, 0, 0 };

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

/* reset_distance - Reset the distance and slot count for both rotary discs.
 */
void reset_distance() {
	char pin_mask = LINT_PIN | RINT_PIN;
	
	EIMSK &= ~pin_mask;	// disable gap-detector interrupts
	left.distance = left.slot_count = 0;
	right.distance = right.slot_count = 0;
	EIMSK |= pin_mask;	// re-enable interrupts
}

/* reset_ldistance - Reset the distance and slot count for the left 
 *     rotary disc.
 */
void reset_ldistance() {
	char pin_mask = LINT_PIN;
	
	EIMSK &= ~pin_mask;	// disable gap-detector interrupts
	left.distance = left.slot_count = 0;
	EIMSK |= pin_mask;	// re-enable interrupts
}

/* reset_rdistance - Reset the distance and slot count for the right 
 *     rotary disc.
 */
void reset_rdistance() {
	char pin_mask = RINT_PIN;
	
	EIMSK &= ~pin_mask;	// disable gap-detector interrupts
	right.distance = right.slot_count = 0;
	EIMSK |= pin_mask;	// re-enable interrupts
}

/* get_distance - Set the longs pointed to by ldist and rdist to the distance
 *     (in cm*10) traveled by the left and right wheels, respectively, since 
 *     the last distance reset.
 */
void get_distance(unsigned long *ldist, unsigned long *rdist) {
	char pin_mask = LINT_PIN | RINT_PIN;
	
	EIMSK &= ~pin_mask;	// disable gap-detector interrupts
	*ldist = left.distance + (left.slot_count * SEGMENT);
	*rdist = right.distance + (right.slot_count * SEGMENT);
	EIMSK |= pin_mask;	// re-enable interrupts
}

/* get_ldistance - Return the distance (in cm*10) traveled by the left 
 *     wheel since its last distance reset.
 */
unsigned long get_ldistance() {
	unsigned long distance;
	char pin_mask = LINT_PIN;
	
	EIMSK &= ~pin_mask;	// disable gap-detector interrupt
	distance = left.distance + (left.slot_count * SEGMENT);
	EIMSK |= pin_mask;	// re-enable interrupt
	
	return distance;
}

/* get_rdistance - Return the distance (in cm*10) traveled by the 
 *     right wheel since its last distance reset.
 */
unsigned long get_rdistance() {
	unsigned long distance;
	char pin_mask = RINT_PIN;
	
	EIMSK &= ~pin_mask;	// disable gap-detector interrupt
	distance = right.distance + (right.slot_count * SEGMENT);
	EIMSK |= pin_mask;	// re-enable interrupt
	
	return distance;
}

/* get_freq - Set the longs pointed to by lfreq and rfreq to the number
 *     of ms elapsed between the last two falling edges of LINPUT_PIN and
 *     RINPUT_PIN, respectively.
 */
void get_freq(unsigned long *lfreq, unsigned long *rfreq) {
	*lfreq = left.msPerSlot;
	*rfreq = right.msPerSlot;
}

/* get_lfreq - Return the number of ms elapsed between the last two 
 *     falling edges of LINPUT_PIN.
 */
unsigned long get_lfreq() {
	return left.msPerSlot;
}

/* get_rfreq - Return the number of ms elapsed between the last two 
 *     falling edges of RINPUT_PIN.
 */
unsigned long get_rfreq() {
	return right.msPerSlot;
}

/* LINPUT_PIN external interrupt - Update left wheel time/distance
*     measurements on falling edges of LINPUT_PIN.
 */
ISR(INT1_vect) {
	static unsigned long prev_time = 0;
	unsigned long curr_time = x_gtime();
	
	left.msPerSlot = curr_time - prev_time;
	prev_time = curr_time;
	
	if (++left.slot_count == NUM_SLOTS) {
		left.distance += CIRC;
		left.slot_count = 0;
	}
}

/* RINPUT_PIN external interrupt - Update right wheel time/distance 
 *     measurements on falling edges of RINPUT_PIN.
 */
ISR(INT2_vect) {
	static unsigned long prev_time = 0;
	unsigned long curr_time = x_gtime();
	
	right.msPerSlot = curr_time - prev_time;
	prev_time = curr_time;
	
	if (++right.slot_count == NUM_SLOTS) {
		right.distance += CIRC;
		right.slot_count = 0;
	}
}
