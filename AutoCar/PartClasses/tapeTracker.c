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

//returns true if black is being seen
//returns false if something visible is there
int offTrack()
{
	if ((PINF & (1 << 0)) == 0)
		return 1;
	else
		return 0;
}