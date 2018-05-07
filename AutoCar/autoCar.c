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
	//x_new(2, blinky, true);
	//x_new(3, obstacle_thread, true);
	//x_new(0, go_forward_thread, true);
	//x_delay(1000);
	//x_new(4, go_forward_thread, true);
	//x_delay(5000);
	//thread 0
    while (1) 
    {
		x_delay(50);
		//go_straight(1000, MED_SPEED, FRWD);
		/*setDirectionForward();
		setRightSpeed(0xFF);
		setLeftSpeed(0xFF)
		//x_delay(1000);*/
		//go_straight(500, MED_SPEED, BKWD);
		//pivot(180);
	}
}

void setUp() {
	init_tracker();
	init_motors();
	init_sensor();
	init_photoInterruptors();
}

/*void blinky() {
	DDRB = 0x80;
	while(1) {
		PORTB = 0x80;
		x_delay(100);
		PORTB = 0x00;
		x_delay(100);
	}
}*/




