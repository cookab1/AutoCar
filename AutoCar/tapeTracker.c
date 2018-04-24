/*
 * TapeTracker.c
 *
 * Created: 4/19/2018 1:38:36 PM
 * Author : john
 */ 

#include <avr/io.h>
#include <util/delay.h>


int initTracker()
{
	DDRF = 0x00;	
}

int onTrack() 
{
	if ((PINF & 0x01) == 0)
		return 1;
	else
		return 0;
}
