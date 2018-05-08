
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
	unsigned int lsteps = 0, rsteps = 0;
	unsigned int ltotal = 0, rtotal = 0, total = 0;
	uint8_t lspeed = (speed >= MIN_SPEED + 6) ? speed - 6 : speed;
	uint8_t rspeed = speed;
	int error;
	int kp = 6;		/* constant of proportionality */

	if (speed < MIN_SPEED)
		return;
	if (direction == BKWD)
		setDirectionBackward();
	else
		setDirectionForward();
	
	reset_steps();
	total = cmToSteps(cm);
	setLeftSpeed(lspeed);
	setRightSpeed(rspeed);
	
	while (ltotal < total || rtotal < total) {
		get_steps(&lsteps, &rsteps);
		ltotal += lsteps;
		rtotal += rsteps;
		
		error = (rsteps - lsteps) / kp;
		
		if (error > 0 && MAX_SPEED - error < lspeed)
			lspeed = MAX_SPEED;
		else if (error < 0 && lspeed + error < MIN_SPEED)
			lspeed = MIN_SPEED;
		else
			lspeed += error;
		
		reset_steps();
		setLeftSpeed(lspeed);
		setRightSpeed(rspeed);		
		x_delay(300);
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
	int kp = 6;		/* constant of proportionality */
	uint8_t lspeed = MED_SPEED - 6;
	uint8_t rspeed = MED_SPEED;

	reset_steps();
	setDirectionForward();
	setRightSpeed(rspeed);
	setLeftSpeed(lspeed);
	
	while (1) {
		get_steps(&lsteps, &rsteps);
		
		error = (rsteps - lsteps) / kp;
		
		if (error > 0 && MAX_SPEED - error < lspeed)
			lspeed = MAX_SPEED;
		else if (error < 0 && lspeed + error < MIN_SPEED)
			lspeed = MIN_SPEED;
		else
			lspeed += error;
		
		setRightSpeed(rspeed);
		setLeftSpeed(lspeed);
		reset_steps();	/* avoid overflow */
		x_delay(300);
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
		degrees = -degrees;
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

/* obstacle_thread - Continuously take readings from the sensor. If a reading
 *     is below 
 */
void obstacle_thread() {
    uint16_t reading;
    uint16_t stopdist = 100; /* 10.0cm */
    uint16_t clearance = 320;   /* min sensor reading to stop pivoting (29.9cm) */
    uint8_t lspeed, rspeed, num_readings = 5;

    while (1) {
        if ((reading = read_avg_sensor(num_readings)) <= stopdist) {
			lspeed = getLeftSpeed();
			rspeed = getRightSpeed();
            stop();
			/* pivot incrementally */
            while (reading < clearance) {
				leftDirectionForward();
				rightDirectionBackward();
				setSpeed(0xC0);
                _delay_ms(300);	/* don't allow other threads to move the car */
                stop();
				reading = read_avg_sensor(10);
            }
            setDirectionForward();
			reset_steps();
			setLeftSpeed(lspeed);
			setRightSpeed(rspeed);
        }
        x_delay(1);
    }
}

/*
 */
void adjustForTrack(int i) {	
	// if too far left
	if (i == 1) {
		//adjust to the right
		pivot(8);
	}
	// if too far right
	else if (i == 2) {
		pivot(-8);
	}
}

/*
 */
void trackListener_thread() {
	int state;
	
	while(1) {		
		state = offTrack();
		
		if (state == 1 || state == 2) {
			stop();
			adjustForTrack(state);
		}
		else {
			setDirectionForward();
			setSpeed(0xB0);
		}

		x_delay(1);
	}
}