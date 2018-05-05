
#include <avr/io.h>
#include <util/delay.h>
#include "navigation.h"
#include "PartClasses/tapeTracker.h"
#include "PartClasses/motorControl.h"
#include "PartClasses/RangingSensor.h"
#include "PartClasses/PhotoInterruptor.h"
#include "HelperClasses/acx.h"

void go_straight(unsigned int cm, uint8_t speed, uint8_t direction) {
	unsigned int lsteps = 0;
	unsigned int rsteps = 0;
	unsigned int steps = 0;
	int error;
	int kp = 5;		/* constant of proportionality */

	if (speed < MIN_SPEED)
		return;
	if (direction == BKWD)
		setDirectionBackward();
	else
		setDirectionForward();

	steps = cmToSteps(cm);
	reset_steps();
	setSpeed(speed);
	
	while (rsteps < steps && lsteps < steps) {
		get_steps(&lsteps, &rsteps);
		
		error = (rsteps - lsteps) / kp;
		
		if (error > 0 && MAX_SPEED - error < speed)
			speed = MAX_SPEED;
		else if (error < 0 && speed + error < MIN_SPEED)
			speed = MIN_SPEED;
		else
			speed += error;
		
		leftSpeed(speed);		
		x_delay(100);
	}

	stop();
}


/**
 * Turns specified number of "degrees" (in quotations because not very accurate)
 * positive value turns right, negative value turns left
 * 
 * At speed A0, 1200ms delay turns about 90 degrees
 */
void pivot(int degrees) {
	if (degrees < 0) {
		rightDirectionForward();
		leftDirectionBackward();
	} else if (degrees > 0) {
		rightDirectionBackward();
		leftDirectionForward();	
	}
	
	setSpeed(0xB0);
	x_delay(degrees * 10);
	stop();
}