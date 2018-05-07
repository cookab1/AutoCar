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
	//x_new(1, trackListener, true);
	x_new(2, blinky, true);
	x_new(3, obstacle_thread, true);
	x_new(0, go_forward_thread, true);
	//x_delay(1000);
	//x_new(4, go_forward_thread, true);
	//x_delay(5000);
	//thread 0
    while (1) 
    {
		x_delay(1000);
		go_straight(2000, MED_SPEED, FRWD);
		x_delay(1000);
		//go_straight(500, MED_SPEED, BKWD);
		//pivot(180);
	}
}
void adjustForTrack(int i) {
	
	// if too far left
	if (i == 1) {
		//adjust to the right
		pivot(4);
	}
	// if too far right
	else if (i == 2) {
		pivot(-4);
	}
	
	// adjust to the left
}

void trackListener() {
	while(1) {
		/*
		if(offTrack() == 1) {
			adjustForTrack(1);
		}
		else if(offTrack() == 2) {
			adjustForTrack(2);
		}
		*/
		int i;
		if(i = offTrack()) {
			adjustForTrack(i);
		} else {
			setDirectionForward();
			setRightSpeed(0xFF);
			setLeftSpeed(0xFF);
		}
		x_delay(5);
	}
}

void setUp() {
	init_tracker();
	init_motors();
	init_sensor();
	init_photoInterruptors();
}

void blinky() {
	DDRB |= 0x80;
	while(1) {
		PORTB |= 0x80;
		x_delay(100);
		PORTB &= 0x7F;
	}
}




