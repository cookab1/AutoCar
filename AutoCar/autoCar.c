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
	
	x_init();
	//initialize threads
	x_new(1, trackListener, true);
	//x_new(2, blinky, true);
	//x_delay(5000);
	//forward1s();
	//backward1s();
	
	setDirectionForward();
	rightSpeed(0xFF);
	leftSpeed(0xFF);
	//thread 0
    while (1) 
    {
		x_delay(100);
		/*
		setDirectionBackward();
		rightSpeed(0xA0);
		leftSpeed(0xA0);
		x_delay(100);
		*/
	}
}

void trackListener() {
	while(1) {
		if(onTrack()) {
			stop();
			//correct alignment
		}
		x_delay(5);
	}
}

void setUp() {
	init_tracker();
	init_motors();
}

void blinky() {
	DDRB = 0x80;
	while(1) {
		PORTB ^= 0x80;
		x_delay(100);
	}
}



