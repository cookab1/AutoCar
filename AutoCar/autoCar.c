/*
 * AutoCar.c
 *
 * Created: 4/24/2018 2:56:04 PM
 * Author : cookab1
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "PartClasses/tapeTracker.h"
#include "PartClasses/motorControl.h"
#include "PartClasses/RangingSensor.h"
#include "autoCar.h"
#include "HelperClasses/acx.h"
#include "HelperClasses/acxserial.h"
#include "HelperClasses/sw_delay.h"

int main(void)
{
	setUp();
	
	//initialize threads
	//x_new(1, trackListener, true);
	//x_delay(5000);
	forward1s();
	//backward1s();
	//thread 0
	DDRB = 0x80;
    while (1) 
    {
		
    }
}

void trackListener() {
	if(!onTrack()) {
		//correct alignment
	}
	x_yield();
}

void setUp() {
	//x_init();
	initTracker();
	init_motors();
}



