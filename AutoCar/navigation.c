
#include <avr/io.h>
#include <util/delay.h>
#include "navigation.h"
#include "PartClasses/tapeTracker.h"
#include "PartClasses/motorControl.h"
#include "PartClasses/RangingSensor.h"
#include "PartClasses/PhotoInterruptor.h"
#include "HelperClasses/acx.h"

/* Uses a proportional algorithm to adjust the speed of the left wheel to match
 * the speed of the right wheel. We'll need to play with different values for 
 * kp and the delay.
 */
void go_straight(unsigned int cm, uint8_t speed, uint8_t direction) {
	unsigned int lsteps = 0;
	unsigned int rsteps = 0;
	unsigned int steps = 0;
	int error;
	int kp = 5;		/* constant of proportionality */
	uint8_t lspeed = speed;
	uint8_t rspeed = speed;

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
			lspeed = MAX_SPEED;
		else if (error < 0 && speed + error < MIN_SPEED)
			lspeed = MIN_SPEED;
		else
			lspeed += error;
		
		rightSpeed(rspeed);
		leftSpeed(lspeed);		
		x_delay(100);
	}

	stop();
}

/* go_forward_thread - Move forward forever. Intended to be used in
 *     conjunction with an obstacle-avoidance or tape-tracking
 *     thread.
 */
void go_forward_thread() {
	unsigned int lsteps = 0;
	unsigned int rsteps = 0;
	int error;
	int kp = 5;		/* constant of proportionality */
	uint8_t lspeed = MED_SPEED;
	uint8_t rspeed = MED_SPEED;

	reset_steps();
	setSpeed(speed);
	
	while (1) {
		get_steps(&lsteps, &rsteps);
		
		error = (rsteps - lsteps) / kp;
		
		if (error > 0 && MAX_SPEED - error < speed)
			lspeed = MAX_SPEED;
		else if (error < 0 && speed + error < MIN_SPEED)
			lspeed = MIN_SPEED;
		else
			lspeed += error;
		
		rightSpeed(rspeed);
		leftSpeed(lspeed);
		x_delay(100);
	}
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

/* Work in progress:
 * We need to work out values for stopdist and clearance.
 * stopdist: sensor reading required to trigger obstacle avoidance pivot
 * clearance: min sensor reading needed to stop pivoting (and go forward).
 * This is the third side of the right triangle formed by 1) the distance 
 * between the middle of the sensor and the outer edge of a wheel (1/2 car width)
 * and 2) the sensor's 30 degree angle of operation. So:
 *    clearance = (1/2 car width) * tan(75)
 * (assuming the sensor is centered on the car)
 */
void obstacle_thread() {
    uint16_t reading;
    uint16_t stopdist = 50; /* 5.0cm */
    uint16_t clearance = 150;   /* min sensor reading to stop pivoting */
    uint8_t num_readings = 5;

    while (1) {
        if ((reading = read_avg_sensor(num_readings)) <= stopdist) {
            stop();
            /* pivot incrementally */
		    rightDirectionForward();
		    leftDirectionBackward();
            while (reading < clearance) {
                setSpeed(0xA0);
                _delay_ms(20);	/* don't allow other threads to move the car */
                stop();
                reading = read_avg_sensor(num_readings);
            }
            setDirectionForward();
        }
        x_delay(1);
    }
}
