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
	x_init();
	setUp();
	x_new(1, trackListener_thread, true);
	//x_new(2, obstacle_thread, true);
	//x_new(3, go_forward_thread, true);
	//x_new(4, go_forward_thread, true);	
	
    while (1) 
    {
		x_delay(50);
	}
}

void setUp() {
	init_tracker();
	init_motors();
	init_sensor();
	init_photoInterruptors();
}

/* straight-line travel 8 seconds
 */
void test_one() {
	unsigned long start_time;
	
	x_delay(1000);
	start_time = x_gtime();
	x_new(1, go_forward_thread, true);
	
	while (x_gtime() - start_time < 8000) {
		x_delay(1);
	}
	
	stop();
	x_disable(1);
}

/* straight-line travel 8 feet
 */
void test_two() {
	unsigned int cm = 2438;	// 243.8cm = 8ft
	
	x_delay(1000);
	go_straight(cm, MED_SPEED, FRWD);
}

/* straight-line travel 8 feet in 30 seconds
 */
void test_three() {
	unsigned int cm = 2438;	// 243.8cm = 8ft
	uint8_t speed = 0xA0;	// 8.1 cm/s ????
		
	go_straight(cm, speed, FRWD);
}

/* Pivot test: Rotate in place: clockwise (+)90, wait 4 seconds, +180, 4s, 
 *    -90, -180
 */
void test_four() {
	pivot(90);
	_delay_ms(4000);
	pivot(300);
	_delay_ms(4000);
	pivot(-100);
	_delay_ms(4000);
	pivot(-100);
}

void blinky() {
	DDRB |= 0x80;
	while(1) {
		PORTB |= 0x80;
		x_delay(100);
		PORTB &= 0x7F;
		x_delay(100);
	}
}




