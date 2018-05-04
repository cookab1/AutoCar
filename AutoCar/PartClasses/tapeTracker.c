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
	if ((PINF & (1 << 0)) == 0)
		return 1; 
	// bit 1 is lefts
	/*else if ((PINF & (1 << 1)) == 0)
		return 2;*/
	else
		return 0;
	/*
	switch(PINF & 0x03) {
		case 0x11:
		case 0x00:
			return 0;
		case 0x10:
			return 1;
		case 0x01:
			return 2;
		case default:
			return 3;
		
	}
	return 0;
	*/
}
