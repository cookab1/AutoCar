/*
 * TapeTracker.c
 *
 * Created: 4/19/2018 1:38:36 PM
 * Author : john
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define RIGHTPIN (1 << PF0)
#define LEFTPIN  (1 << PF1)

void init_tracker()
{
	// set pins for input
	DDRF &= ~(RIGHTPIN | LEFTPIN);
}

// reading 0 is seeing tape
// reading 1 is seeing ground

//returns true if black is being seen
//returns false if something visible is there
int offTrack()
{
	// check if on the right
	if ((PINF & 0x03) == RIGHTPIN) {
		return 1; 
	}
	// check if on the left
	if ((PINF & 0x03) == LEFTPIN) {
		return 2;
	}
	// if not obstacle on left or right return 0. Also returns 0 for both.
	else
		return 0;
}
