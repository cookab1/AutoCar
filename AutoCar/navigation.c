
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
	
	if (direction == BKWD)
		setDirectionBackward();
	else
		setDirectionForward();
		
	reset_steps();
	setSpeed(speed);
	
	while (lsteps < steps && rsteps < steps) {	// || ?
		get_steps(&lsteps, &rsteps);
		
		if (rsteps > lsteps) {
			if (rspeed > 0)
				rightSpeed(--rspeed);
			else
				leftSpeed(++lspeed);
		}
		else if (rsteps < lsteps) {
			if (rspeed < 0xFF)
				rightSpeed(++rspeed);
			else
				leftSpeed(--lspeed);
		}
		
		x_delay(1);
	}

	stop();
}
