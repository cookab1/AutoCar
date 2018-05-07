/*
 * TapeTracker.c
 *
 * Created: 4/19/2018 1:38:36 PM
 * Author : john
 */ 

#include <avr/io.h>
#include <util/delay.h>

int pinUsed;

void init_tracker()
{
	//pinUsed = pin;
	DDRF = 0x00;
}

// reading 0 is seeing tape
// reading 1 is seeing ground

//returns true if black is being seen
//returns false if something visible is there
int offTrack()
{
	// check if on the right
	if ((PINF & 0x03) == 0x01) {
		return 1; 
	}
	// check if on the left
	if ((PINF & 0x03) == 0x02) {
		return 2;
	}
	// if not obstacle on left or right return 0. Also returns 0 for both.
	else
		return 0;
}
