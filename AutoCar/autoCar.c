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
#include "PartClasses/PhotoInterruptor.h"
#include "autoCar.h"
#include "HelperClasses/acx.h"
#include "HelperClasses/acxserial.h"
#include "HelperClasses/sw_delay.h"
#include "navigation.h"

int main(void)
{
	setUp();
	
	x_init();
	//initialize threads
	x_new(1, trackListener, true);
	x_new(2, blinky, true);
	//x_delay(5000);
	//forward1s();
	//backward1s();
	
	//setDirectionForward();
	//rightSpeed(0xFF);
	//leftSpeed(0xFF);
	//thread 0
    while (1) 
    {
		go_straight(100, 0x80, FRWD);
		x_delay(5000);
		go_straight(100, 0x80, BKWD);
		x_delay(5000);
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
		if(offTrack()) {
			//stop();
			//correct alignment
		}
		x_delay(5);
	}
}

void setUp() {
	init_tracker();
	init_motors();
	init_photoInterruptors();
}

void blinky() {
	DDRB = 0x80;
	while(1) {
		PORTB ^= 0x80;
		x_delay(100);
	}
}



