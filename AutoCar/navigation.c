
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
	unsigned int steps = cmToSteps(cm);
	uint8_t lspeed = speed;
	uint8_t rspeed = speed;
	
	if (speed < MIN_SPEED)
		return;
	if (direction == BKWD)
		setDirectionBackward();
	else
		setDirectionForward();
		
	reset_steps();
	setSpeed(speed);
	
	while (lsteps < steps && rsteps < steps) {
		get_steps(&lsteps, &rsteps);
		
		if (rsteps > lsteps) {
			if (rspeed > MIN_SPEED)
				rightSpeed(--rspeed);
			else
				leftSpeed(++lspeed);
		}
		else if (rsteps < lsteps) {
			if (rspeed < MAX_SPEED)
				rightSpeed(++rspeed);
			else
				leftSpeed(--lspeed);
		}
		
		x_delay(1);
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