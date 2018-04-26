/*
 * AutoCar.c
 *
 * Created: 4/24/2018 2:56:04 PM
 * Author : cookab1
 */ 

#include <avr/io.h>
#include "tapeTracker.h"
#include "motorControl.h"
#include "autoCar.h"

int main(void)
{
	setUp()
	
	//initialize threads
	x_new(1, trackListener, true);
	
	//thread 0
    while (1) 
    {
		
    }
}

void trackListener() {
	if(!onTrack()) {
		//correct alignment
	}
}

void setUp() {
	x_init();
	initTracker();
}



