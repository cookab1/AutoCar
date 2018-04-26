/*
 * MotorControl.c
 *
 * Timer 4 - Timer for right wheel
 * ENA - Speed for right wheel
 * IN1, IN2 - Direction for right wheel
 * 
 * IN3, IN4 - Direction for left wheel
 * ENB - Speed for left wheel
 * Timer 3 - Timer for left wheel
 *
 * Created: 4/19/2018 11:34:38 AM
 * Author : meloanmm
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "../HelperClasses/acx.h"
#include "../HelperClasses/acxserial.h"
#include "motorControl.h"

void forward1s() {
	setDirectionForward();
	rightSpeed(0xFF);
	leftSpeed(0xFF);
	//x_delay(1000);
	_delay_ms(5000);
	stop();
}

void backward1s() {
	setDirectionBackward();
	rightSpeed(0xA0);
	leftSpeed(0xA0);
	//x_delay(1000);
	_delay_ms(1000);
	stop();
}

/**
 * Direction forward for both wheels
 */
void setDirectionForward() {
	PORTC |= (1 << IN1);
	PORTC &= ~(1 << IN2);
	PORTC &= ~(1 << IN3);
	PORTC |= (1 << IN4);
}

/**
 * Direction backward for both wheels
 */
void setDirectionBackward() {
	PORTC &= ~(1 << IN1);
	PORTC |= (1 << IN2);
	PORTC |= (1 << IN3);
	PORTC &= ~(1 << IN4);
	
}

/**
 * Set speed of both wheels
 *
 * Accepted values for motor range from 0x90 (low) to 0xFF (max).
 * This is due to motor having a minimum accepted voltage of around 6V.
 * A minimum value just above half is to make sure inconsistency in voltage
 * won't just stop the wheel suddenly.
 */
void setSpeed(uint8_t percent) {
	OCR4A = percent;
	OCR3A = percent;
}

/**
 * Stops wheels from turning, and sets speed to 0 (turns off signal to save power)
 */
void stop() {
	PORTC &= 0xF0;
	setSpeed(0x00);
}

/**
 * Setup timers and PWM settings for wheel speed control
 */
void init_motors() {	
	//porth 3 - OC4A - timer 4 - ena - pin 6
	//porte 3 - OC3A - timer 3 - enb - pin 5
	
	
	//ena, enb set to output
	DDRH |= (1 << PINH3);
	DDRE |= (1 << PINE3);
	//in1, in2, in3, in4 set to output
	// a8, a9, a10, a11    /   dig pin 37, 36, 35, 34   / pin 53, 54, 55, 56
	DDRC &= 0x0F;
	
	
	
	//////////////// PWM

	
	// set COMnA1 : set at bottom, clear at match
	TCCR4A |= (1 << COM4A1) | (1 << WGM40);
	TCCR4B |= (1 << WGM42);
	
	TCCR3A |= (1 << COM3A1) | (1 << WGM30);
	TCCR3B |= (1 << WGM32);
	
	
	
	// no prescaling
	TCCR4B |= (1 << CS40);
	TCCR3B |= (1 << CS30);
	
	//set output compare to 0
	OCR4A = 0;
	OCR3A = 0;
}

void rightDirectionForward() {
	PORTC |= (1 << IN1);
	PORTC &= ~(1 << IN2);
}

void leftDirectionForward() {
	PORTC &= ~(1 << IN3);
	PORTC |= (1 << IN4);
}

void rightDirectionBackward() {
	PORTC &= ~(1 << IN1);
	PORTC |= (1 << IN2);
	
}

void leftDirectionBackward() {
	PORTC |= (1 << IN3);
	PORTC &= ~(1 << IN4);
}

/**
 * Set speed for right wheel as percentage of power.
 *
 * Acceptable range for percent value: 0x90 to 0xFF
 * See setSpeed for full explanation.
 */
void rightSpeed (uint8_t percent) {
	OCR4A = percent;
}

/**
 * Set speed for left wheel as percentage of power.
 *
 * Acceptable range for percent value: 0x90 to 0xFF
 * See setSpeed for full explanation.
 */
void leftSpeed (uint8_t percent) {
	OCR3A = percent;
}

void rightStop() {
	PORTC &= 0xFC;
}

void leftStop() {
	PORTC &= 0xF3;
}